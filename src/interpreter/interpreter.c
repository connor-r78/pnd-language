#include "./interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parser/parse.h"
#include "./builtins.h"
#include "./env.h"
#include "./value.h"

interpreter_t* init_interpreter() {
  interpreter_t* ret = malloc(sizeof(interpreter_t));
  ret->env = env_init(1000);
  init_builtins(ret->env);
  return ret;
}

void interpreter_free(interpreter_t* interp) {
  env_free(interp->env);
  free(interp);
}

// Forward decleration
Value eval_value(interpreter_t* interp, Value value);

const Value nil = {VALUE_NIL, .length = 0};

Value builtin_set(size_t argc, Value* argv, interpreter_t* interp) {
  if (argc != 2) {
    return nil;
  }
  if (argv[0].type != VALUE_SYMBOL) {
    return nil;
  }

  Value evaluated = eval_value(interp, argv[1]);
  env_add(interp->env, argv[0].as.symbol, evaluated);
  return evaluated;
}
Value eval_value(interpreter_t* interp, Value value) {
  switch (value.type) {
    case VALUE_NIL:
    case VALUE_NUMBER:
    case VALUE_STRING:
    case VALUE_CFUNC:
    case VALUE_LAMBDA:
      return value;

    case VALUE_SYMBOL: {
      return env_lookup(interp->env, value.as.symbol);
    }

    case VALUE_LIST: {
      if (value.length == 0 || !value.as.list) {
        return nil;
      }

      Value first = value.as.list->value;

      if (first.type == VALUE_SYMBOL) {
        const char* funcname = first.as.symbol;
        size_t argc = value.length - 1;

        if (strcmp(funcname, "lambda") == 0) {
          if (argc >= 2) {
            LambdaFunction* l = malloc(sizeof(LambdaFunction));
            ValueList* params_node = value.as.list->next;
            Value params_val = params_node->value;
            if (params_val.type == VALUE_LIST) {
              params_val = eval_value(interp, params_val);
            }
            if (params_val.type == VALUE_LIST) {
              l->args = params_val.as.list;
            } else {
              l->args = NULL;
            }

            ValueList* body_src = params_node->next;
            l->body = NULL;
            ValueList** body_tail = &l->body;
            ValueList* cur = body_src;
            while (cur) {
              Value v = cur->value;
              if (v.type == VALUE_LIST) {
                v = eval_value(interp, v);
              }
              ValueList* new_node = malloc(sizeof(ValueList));
              new_node->value = v;
              new_node->next = NULL;
              *body_tail = new_node;
              body_tail = &new_node->next;
              cur = cur->next;
            }

            return (Value){.type = VALUE_LAMBDA, .as.lambda = l};
          }
        }

        if (strcmp(funcname, "quote") == 0) {
          if (argc < 1) {
            return nil;
          }
          ValueList* current = value.as.list->next;
          if (!current) {
            return nil;
          }
          return current->value;
        }

        if (strcmp(funcname, "set") == 0) {
          if (argc != 2) {
            return nil;
          }

          Value* argv = malloc(2 * sizeof(Value));
          ValueList* current = value.as.list->next;

          argv[0] = current->value;
          current = current->next;

          argv[1] = eval_value(interp, current->value);

          Value result = builtin_set(argc, argv, interp);

          free(argv);

          return result;
        }
      }

      Value op = eval_value(interp, first);
      size_t argc = value.length - 1;
      Value* argv = NULL;
      if (argc > 0) {
        argv = malloc(argc * sizeof(Value));
        ValueList* current = value.as.list->next;
        for (size_t i = 0; i < argc && current; i++) {
          argv[i] = eval_value(interp, current->value);
          current = current->next;
        }
      }

      Value result;
      result.type = VALUE_NIL;
      result.length = 0;

      if (op.type == VALUE_CFUNC) {
        result = op.as.cfunc(argc, argv);
        if (argv)
          free(argv);
        return result;
      }

      if (op.type == VALUE_LAMBDA) {
        LambdaFunction* l = op.as.lambda;
        ValueList* params = l->args;
        size_t i = 0;
        Value res = nil;
        // handle actually passing args later, just create them in the env to
        // avoid problems
        ValueList* p = params;
        while (p && i < argc) {
          if (p->value.type == VALUE_SYMBOL) {
            env_add(interp->env, p->value.as.symbol, argv[i]);
          }
          i++;
          p = p->next;
        }

        ValueList* body = l->body;
        while (body) {
          res = eval_value(interp, body->value);
          body = body->next;
        }

        if (argv)
          free(argv);
        return res;
      }

      if (argv)
        free(argv);
      return nil;
    }
  }

  return nil;
}

Value eval_sexp(interpreter_t* interp, SExp* input) {
  if (!input) {
    return nil;
  }

  Value value = sexp_to_value(input);
  Value result = eval_value(interp, value);
  return result;
}
