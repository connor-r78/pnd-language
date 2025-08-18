#include "./interpreter.h"
#include "./env.h"
#include "../parser/parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interpreter_t* init_interpreter() {
  interpreter_t* ret = malloc(sizeof(interpreter_t));
  ret->env = env_init(1000);

  return ret;
}
void interpreter_free(interpreter_t* interp) {
  env_free(interp->env);
  free(interp);
}
// testing func untill there is a proper env and interpterter
SExp testing_func(size_t argc, SExp** argv) {
  printf("testing func called, argc: %lu ", argc);
  for (size_t i = 0; i < argc; i++) {
    printf(" argument %lu: ", i);
    print_sexp(argv[i]);
  }
  printf("\n");
  return *argv[0];
}
SExp println(size_t argc, SExp** argv) {
  for (size_t i = 0; i < argc; i++) {
    print_sexp(argv[i]);
    printf(" ");
  }
  printf("\n");
  SExp nil_sexp;
  nil_sexp.type = SEXP_NIL;
  return nil_sexp;
}
SExp add(size_t argc, SExp** argv) {
  double sum = 0;
  for (size_t i = 0; i < argc; i++) {
    if (argv[i]->type == SEXP_NUMBER) {
      sum += argv[i]->as.number;
    }
  }
  SExp result;
  result.type = SEXP_NUMBER;
  result.as.number = sum;
  result.length = 0;  // not a list
  return result;
}
SExp set(size_t argc, SExp** argv, interpreter_t* interp) {
  if (argc != 2) {
    SExp nil_sexp;
    nil_sexp.type = SEXP_NIL;
    return nil_sexp;
  }
  if (argv[0]->type != SEXP_SYMBOL) {
    SExp nil_sexp;
    nil_sexp.type = SEXP_NIL;
    return nil_sexp;
  }

  env_add(interp->env, argv[0]->as.symbol, argv[1]);
  return *argv[1];
}

SExp eval_sexp(interpreter_t* interp, SExp* input) {
  if (!input) {
    SExp nil_sexp;
    nil_sexp.type = SEXP_NIL;
    return nil_sexp;
  }
  // TODO: fix redundant checking, ideally the checking would happen before call
  // to allow for easier recursion;
  if (input->type != SEXP_LIST) {
    if (input->type == SEXP_SYMBOL) {
      SExp* found = env_lookup(interp->env, input->as.symbol);
      if (found) {
        return *found;
      } else {
        SExp nil_sexp;
        nil_sexp.type = SEXP_NIL;
        return nil_sexp;
      }
    } else {
      return *input;
    }
  }
  if (!input->as.list || !input->as.list->value) {
    SExp nil_sexp;
    nil_sexp.type = SEXP_NIL;
    return nil_sexp;
  }
  SExp* first = input->as.list->value;

  if (first->type != SEXP_SYMBOL) {
    return *input;
  }
  const size_t argc = input->length - 1;
  const char* funcname = first->as.symbol;
  if (strcmp(funcname, "set") == 0) {
    SExp** argv = malloc(argc * sizeof(SExp*));
    SExpList* current = input->as.list->next;
    // For set, don't evaluate the first argument.
    argv[0] = current->value;
    current = current->next;
    // Evaluate the second argument.
    SExp result_val = eval_sexp(interp, current->value);
    // The result must be heap-allocated to be stored in the environment.
    SExp* result_ptr = malloc(sizeof(SExp));
    *result_ptr = result_val;
    argv[1] = result_ptr;

    SExp final_result = set(argc, argv, interp);
    free(argv);
    return final_result;
  }

  SExp** argv = malloc(argc * sizeof(SExp*));
  size_t i = 0;
  SExpList* current = input->as.list->next;
  while (i < argc) {
    SExp result_val = eval_sexp(interp, current->value);
    SExp* result_ptr = malloc(sizeof(SExp));
    *result_ptr = result_val;
    argv[i] = result_ptr;
    current = current->next;
    i++;
  }

  SExp final_result;
  if (!strcmp(funcname, "println"))
    final_result = println(argc, argv);
  else if (!strcmp(funcname, "add"))
    final_result = add(argc, argv);
  else
    final_result = testing_func(argc, argv);

  // Free the malloced arguments
  for (size_t j = 0; j < argc; j++) {
    free(argv[j]);
  }
  free(argv);

  return final_result;
}
