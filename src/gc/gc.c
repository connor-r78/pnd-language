#include "./gc.h"

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ADDRESS_SIZE = 8

struct GCObject {
  void* memAddress;
  int color;
};

void* memAddress[0];

void gc_free(void** ptr) {
  free(*ptr);
  *ptr = NULL;
}

void push_gc_object(struct GCObject *object) {
  int memAddressLength = sizeof(memAddress)/8;

  void* tempMemAddress[memAddressLength+1];

  for (int i = 0; i < memAddressLength; i++) {
    tempMemAddress[i] = memAddress[i];
  }

  tempMemAddress[memAddressLength] = object->memAddress;
}

void* gc_alloc(size_t size) {
  int* ptr = malloc(size);

  struct GCObject gcObject = {ptr, 0};

  push_gc_object(&gcObject);
  
  return ptr;
}