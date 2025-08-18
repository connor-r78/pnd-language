/*

Using the functions in this header file:

Defining the bottom of the stack:

int stack_bottom;
gc_init(&stack_bottom);

Triggering garbage collection:

gc_collect();

*/

#ifndef GC_H
#define GC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void gc_init(void *stack_bottom_hint);

void gc_register_root(void **ptr_location);

void *gc_alloc(size_t size);

void gc_collect(void);

#define GC_NEW(T) ((T *)gc_alloc(sizeof(T)))

#ifdef __cplusplus
}
#endif

#endif