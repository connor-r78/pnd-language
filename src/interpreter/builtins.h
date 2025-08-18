#ifndef PND_BUILTINS_H
#define PND_BUILTINS_H

#include "./interpreter.h"
#include "./env.h"
#include "./value.h"

Value builtin_println(size_t argc, Value* argv);
Value builtin_add(size_t argc, Value* argv);

void init_builtins(Env* env);

#endif
