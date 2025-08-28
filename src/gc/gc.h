#ifndef PND_GC_H_
#define PND_GC_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void gc_free(void** ptr);

void* gc_alloc(size_t size);

#define GC_NEW(T) ((T*)gc_alloc(sizeof(T)))

#ifdef __cplusplus
}
#endif

#endif
