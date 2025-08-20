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
  if (!env)
    return;

  for (size_t i = 0; i < env->capacity; i++) {
    EnvEntry_t* current = env->buckets[i];
    while (current) {
      EnvEntry_t* next = current->next;
      free(current->key);
      value_free(&current->value);
      free(current);
      current = next;
    }
  }

  free(env->buckets);
  free(env);
}

void env_add(Env* env, const char* key, Value value) {
  if (!env || !key)
    return;

  uint32_t h = hash(key);
  size_t index = h % env->capacity;
  EnvEntry_t* ent = env->buckets[index];
  EnvEntry_t* prev = NULL;

  while (ent) {
    if (strcmp(ent->key, key) == 0) {
      value_free(&ent->value);
      ent->value = value;
      return;
    }
    prev = ent;
    ent = ent->next;
  }

  EnvEntry_t* new_entry = malloc(sizeof(EnvEntry_t));
  new_entry->key = malloc(strlen(key) + 1);
  strcpy(new_entry->key, key);
  new_entry->value = value;
  new_entry->next = NULL;

  if (prev) {
    prev->next = new_entry;
  } else {
    env->buckets[index] = new_entry;
  }
}

Value env_lookup(Env* env, const char* key) {
  if (!env || !key)
    return (Value){.type = VALUE_NIL};

  uint32_t h = hash(key);
  size_t index = h % env->capacity;
  EnvEntry_t* ent = env->buckets[index];

  while (ent) {
    if (!strcmp(ent->key, key)) {
      return ent->value;
    }
    ent = ent->next;
  }

  return (Value){.type = VALUE_NIL};
}
