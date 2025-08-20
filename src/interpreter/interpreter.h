#ifndef PND_INTERPRETER_H_
#define PND_INTERPRETER_H_
#include "../parser/parse.h"
#include "./env.h"
#include "./value.h"

typedef struct {
  Env* env;
} interpreter_t;

Value eval_sexp(interpreter_t* interp, SExp* input);
Value sexp_to_value(const SExp* sexp);
interpreter_t* init_interpreter(void);
void interpreter_free(interpreter_t* interp);

#endif

