#ifndef PND_ENV_H
#define PND_ENV_H
#include <stdlib.h>
#include "value.h"

typedef struct Entry {
    char *key;
    Value value;
    struct Entry *next;  // linked list for collision
} EnvEntry_t;

typedef struct {
    EnvEntry_t **buckets;
    size_t capacity;
} Env;

Env *env_init(size_t capacity);
void env_free(Env* env);

void env_add(Env* env, const char* key, Value value);
Value env_lookup(Env *env, const char* key);

#endif
