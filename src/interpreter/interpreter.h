#ifndef PND_INTERPRETER_H
#define PND_INTERPRETER_H
#include "../parser/parse.h"
#include "./env.h"
typedef struct {
	Env* env;
} interpreter_t;
SExp* eval_sexp(interpreter_t *interp, SExp *input);
interpreter_t *init_interpreter();
void interpreter_free(interpreter_t* interp);

#endif
