#include "../interpreter/env.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

#define FNV_32_PRIME ((uint32_t)0x01000193)
#define FNV_32_INTNUM ((uint32_t)0x811c9dc5)

uint32_t hash(const char* str) {
  unsigned char* ch = (unsigned char*)str;
  uint32_t hval = FNV_32_INTNUM;

  while (*ch) {
    hval ^= (uint32_t)*ch++;
    hval *= FNV_32_PRIME;
  }
  return hval;
}

Env* env_init(size_t capacity) {
  Env* ret = malloc(sizeof(Env));
  ret->capacity = capacity;
  ret->buckets = calloc(capacity, sizeof(EnvEntry_t*));
  return ret;
}

void env_free(Env* env) {
  free(env->buckets);
  free(env);
}

void env_add(Env* env, char* key, SExp* item) {
  if (!item || !key)
    return;
  uint32_t h = hash(key);
  size_t index = h % env->capacity;
  EnvEntry_t* ent = env->buckets[index];
  EnvEntry_t* prev = NULL;
  while (ent) {
    if (strcmp(ent->key, key) == 0) {
      ent->value = item;
      return;
    }
    prev = ent;
    ent = ent->next;
  }
  EnvEntry_t* new_entry = malloc(sizeof(EnvEntry_t));
  new_entry->key = key;
  new_entry->value = item;
  new_entry->next = NULL;

  if (prev) {
    prev->next = new_entry;
  } else {
    env->buckets[index] = new_entry;
  }
}

SExp* env_lookup(Env* env, const char* key) {
  uint32_t h = hash(key);
  size_t index = h % env->capacity;
  EnvEntry_t* ent = env->buckets[index];
  while (ent) {
    if (!strcmp(ent->key, key)) {
      return ent->value;
    }
    ent = ent->next;
  }
  return NULL;
}
