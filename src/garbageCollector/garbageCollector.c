#include "garbageCollector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct GCObject {
    struct GCObject *next;
    size_t size;
    unsigned char marked;
} GCObject;

static GCObject *gc_head = NULL;
static size_t gc_bytes_allocated = 0;
static size_t gc_threshold = 1 << 20;
static void *gc_stack_bottom = NULL;

#define GC_MAX_ROOTS 256
static void **gc_root_addrs[GC_MAX_ROOTS];
static size_t gc_root_count = 0;

static inline void *payload_of(GCObject *obj) { return (void *)(obj + 1); }

void gc_register_root(void **ptr_location) {
    assert(gc_root_count < GC_MAX_ROOTS);
    gc_root_addrs[gc_root_count++] = ptr_location;
}

void gc_init(void *stack_bottom_hint) {
    gc_stack_bottom = stack_bottom_hint;
}

static void gc_mark_ptr(void *p);
static void gc_mark_object(GCObject *obj);

static void gc_mark_ptr(void *p) {
    uintptr_t addr = (uintptr_t)p;
    for (GCObject *o = gc_head; o; o = o->next) {
        uintptr_t start = (uintptr_t)payload_of(o);
        uintptr_t end = start + o->size;
        if (addr >= start && addr < end) {
            gc_mark_object(o);
            return;
        }
    }
}

static void gc_scan_range(uintptr_t *begin, uintptr_t *end) {
    for (uintptr_t *w = begin; w < end; ++w) {
        gc_mark_ptr((void *)(*w));
    }
}

static void gc_mark_object(GCObject *obj) {
    if (!obj || obj->marked) return;
    obj->marked = 1;
    uintptr_t *begin = (uintptr_t *)payload_of(obj);
    uintptr_t *end = (uintptr_t *)((uint8_t *)payload_of(obj) + obj->size);
    gc_scan_range(begin, end);
}

static void gc_mark_from_roots(void) {
    volatile int stack_var = 0;
    uintptr_t *top = (uintptr_t *)&stack_var;
    uintptr_t *bottom = (uintptr_t *)gc_stack_bottom;
    if (bottom && bottom > top) {
        gc_scan_range(top, bottom);
    }
    for (size_t i = 0; i < gc_root_count; ++i) {
        void *p = *gc_root_addrs[i];
        if (p) gc_mark_ptr(p);
    }
}

static size_t gc_sweep(void) {
    size_t freed_bytes = 0;
    GCObject **link = &gc_head;
    while (*link) {
        GCObject *o = *link;
        if (!o->marked) {
            *link = o->next;
            freed_bytes += sizeof(GCObject) + o->size;
            gc_bytes_allocated -= sizeof(GCObject) + o->size;
            free(o);
        } else {
            o->marked = 0;
            link = &o->next;
        }
    }
    return freed_bytes;
}

void gc_collect(void) {
    gc_mark_from_roots();
    size_t freed = gc_sweep();
    size_t live_estimate = gc_bytes_allocated;
    size_t new_threshold = live_estimate * 2;
    if (new_threshold < (1u << 20)) new_threshold = (1u << 20);
    gc_threshold = new_threshold;
    fprintf(stderr, "[GC] freed ~%zu bytes, heap now ~%zu bytes, next threshold ~%zu\n",
            freed, gc_bytes_allocated, gc_threshold);
}

void *gc_alloc(size_t size) {
    if (!gc_stack_bottom) {
        volatile int bottom_probe = 0;
        gc_init((void *)&bottom_probe);
    }
    if (gc_bytes_allocated + size > gc_threshold) {
        gc_collect();
    }
    GCObject *o = (GCObject *)malloc(sizeof(GCObject) + size);
    if (!o) {
        gc_collect();
        o = (GCObject *)malloc(sizeof(GCObject) + size);
        if (!o) {
            fprintf(stderr, "fatal: out of memory allocating %zu bytes\n", size);
            exit(1);
        }
    }
    o->size = size;
    o->marked = 0;
    o->next = gc_head;
    gc_head = o;
    gc_bytes_allocated += sizeof(GCObject) + size;
    void *payload = payload_of(o);
    memset(payload, 0, size);
    return payload;
}
