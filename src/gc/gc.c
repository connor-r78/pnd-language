#include "./gc.h"

#include <stdio.h>
#include <stdlib.h>

struct GCObject {
  void* memAdd;
  int color;
};

void gc_free(void** ptr) {
  free(*ptr);
  *ptr = NULL;
}

void* gc_alloc(size_t size) {
  int* ptr = malloc(size);
  
  return ptr;
}