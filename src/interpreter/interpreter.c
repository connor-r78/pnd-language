#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/parse.h"
#include "../interpreter/env.h"
#include "./interpreter.h"
interpreter_t* init_interpreter() {
	 interpreter_t *ret = malloc(sizeof(interpreter_t));
	ret->env = env_init(1000);

	return ret;

}
// testing func untill there is a proper env and interpterter
SExp* testing_func(size_t argc, SExp** argv) {
  printf("testing func called, argc: %lu ", argc);
  for (size_t i = 0; i < argc; i++) {
    printf(" argument %lu: ", i);
    print_sexp(argv[i]);
  }
  printf("\n");
  return argv[0];
}
SExp* println(size_t argc, SExp** argv) {
  for (size_t i = 0; i < argc; i++) {
    print_sexp(argv[i]);
    printf(" ");
  }
  printf("\n");
  return NULL;
}
SExp* add(size_t argc, SExp** argv) {
  double sum = 0;
  for (size_t i = 0; i < argc; i++) {
    if (argv[i]->type == SEXP_NUMBER) {
      sum += argv[i]->as.number;
    }
  }
  SExp* result = malloc(sizeof(SExp));
  result->type = SEXP_NUMBER;
  result->as.number = sum;
  result->length = 0;  // not a list
  return result;
}
SExp* set(size_t argc, SExp** argv, interpreter_t* interp) {
		if (argc != 2)
			return NULL;
		if(argv[0]->type != SEXP_SYMBOL)
			return NULL;

		env_add(interp->env, argv[0]->as.symbol, argv[1]);	
	return argv[1];
		}


SExp* eval_sexp(interpreter_t *interp, SExp *input) {
  if (!input)
    return NULL;
  // TODO: fix redundant checking, ideally the checking would happen before call
  // to allow for easier recursion;
  if (input->type != SEXP_LIST) {
	if(input->type == SEXP_SYMBOL) {
			return env_lookup(interp->env, input->as.symbol);
	} else {
    return input;
		}
  }
  if (!input->as.list || !input->as.list->value) {
    return NULL;
  }
  SExp* first = input->as.list->value;

  if (first->type != SEXP_SYMBOL) {
    return input;
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
    argv[1] = eval_sexp(interp, current->value);
    return set(argc, argv, interp);
  }

  SExp** argv = malloc(input->length * sizeof(SExp*));
  size_t i = 0;
  SExpList* current = input->as.list->next;
  while (i < input->length - 1) {
    argv[i] = eval_sexp(interp, current->value);
    current = current->next;
    i++;
  }
  if (!strcmp(funcname, "println"))
    return println(argc, argv);
  else if (!strcmp(funcname, "add"))
    return add(argc, argv);
  else
    return testing_func(argc, argv);
}
