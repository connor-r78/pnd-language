#ifndef PND_ENV_H
#define PND_ENV_H
#include <stdlib.h>
#include "../parser/parse.h"

typedef struct Entry {
    char *key;
    SExp *value;
    struct Entry *next;  // linked list for collission
} EnvEntry_t;

typedef struct {
    EnvEntry_t **buckets;
    size_t capacity;
} Env;

Env *env_init(size_t capacity);
void env_free(Env* env);

void env_add(Env* env, char* key, SExp* item);
SExp* env_lookup (Env *env, const char* key);

#endif
