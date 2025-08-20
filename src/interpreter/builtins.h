#ifndef PND_INTERPRETER_BUILTINS_H
#define PND_INTERPRETER_BUILTINS_H

#include "./env.h"
#include "./interpreter.h"
#include "./value.h"

Value builtin_println(size_t argc, Value* argv);
Value builtin_add(size_t argc, Value* argv);

void init_builtins(Env* env);

#endif
