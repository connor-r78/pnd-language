/*
 * This file contains all the builtin normal functions
 * (those which dont need special argument handling)
 * and their addition to the enviornment in init_builtins.
 */
#include "builtins.h"

#include <stdio.h>

#include "env.h"
#include "value.h"

// if this affects startup time too much we can run this at build time and save
// to a binary, then embed or link the result into the final executable
void init_builtins(Env* env) {
  // TODO: move them here
  env_add(
      env, "println",
      (Value){.type = VALUE_CFUNC, .as.cfunc = &builtin_println, .length = 0});
  env_add(env, "add",
          (Value){.type = VALUE_CFUNC, .as.cfunc = &builtin_add, .length = 0});
}

Value builtin_println(size_t argc, Value* argv) {
  for (size_t i = 0; i < argc; i++) {
    value_print(&argv[i]);
    if (i < argc - 1) {
      printf(" ");
    }
  }
  printf("\n");
  Value nil = {VALUE_NIL, .length = 0};
  return nil;
}

Value builtin_add(size_t argc, Value* argv) {
  double sum = 0.0;
  for (size_t i = 0; i < argc; i++) {
    if (argv[i].type == VALUE_NUMBER) {
      sum += argv[i].as.number;
    }
  }
  Value result = {VALUE_NUMBER, .as.number = sum, .length = 0};
  return result;
}
