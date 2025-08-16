#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/parse.h"
// testing func untill there is a proper env and interpterter
SExp* testing_func(int argc, SExp** argv) {
  printf("testing func called, argc: %d ", argc);
  for (int i = 0; i < argc; i++) {
    printf(" argument %d: ", i);
    print_sexp(argv[i]);
  }
  printf("\n");
  return argv[0];
}
SExp* println(int argc, SExp** argv) {
  for (int i = 0; i < argc; i++) {
    print_sexp(argv[i]);
    printf(" ");
  }
  printf("\n");
  return NULL;
}
SExp* add(int argc, SExp** argv) {
  double sum = 0;
  for (int i = 0; i < argc; i++) {
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
SExp* eval_sexp(SExp* input) {
  if (!input)
    return NULL;
  // TODO: fix redundant checking, ideally the checking would happen before call
  // to allow for easier recursion;
  if (input->type != SEXP_LIST) {
    return input;
  }
  if (!input->as.list || !input->as.list->value) {
    return NULL;
  }
  SExp* first = input->as.list->value;

  if (first->type != SEXP_SYMBOL) {
    return input;
  }
  const int argc = input->length - 1;
  const char* funcname = first->as.symbol;
  SExp** argv = malloc(input->length * sizeof(SExp*));
  int i = 0;
  SExpList* current = input->as.list->next;
  while (i < input->length - 1) {
    argv[i] = eval_sexp(current->value);
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
