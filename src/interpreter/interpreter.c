#include "./interpreter.h"
#include "./env.h"
#include "./value.h"
#include "../parser/parse.h"
#include "./builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Value builtin_set(size_t argc, Value* argv, interpreter_t* interp) {
  if (argc != 2) {
    Value nil = {VALUE_NIL, .length = 0};
    return nil;
  }
  if (argv[0].type != VALUE_SYMBOL) {
    Value nil = {VALUE_NIL, .length = 0};
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
      return value;
      
    case VALUE_SYMBOL:
      {
        Value found = env_lookup(interp->env, value.as.symbol);
        return found;
      }
      
    case VALUE_LIST:
      {
        if (value.length == 0 || !value.as.list) {
          Value nil = {VALUE_NIL, .length = 0};
          return nil;
        }
        
        Value first = value.as.list->value;
        if (first.type != VALUE_SYMBOL) {
          return value;
        }
        
        const char* funcname = first.as.symbol;
        size_t argc = value.length - 1;
        // handle special functions which cant have all arguments looked up/evaluated 
        if (strcmp(funcname, "quote") == 0) {
          if (argc < 1) {
            Value nil = {VALUE_NIL, .length = 0};
            return nil;
          }
          ValueList* current = value.as.list->next;
          if (!current) {
            Value nil = {VALUE_NIL, .length = 0};
            return nil;
          }
          return current->value;
        }
        
        if (strcmp(funcname, "set") == 0) {
          if (argc != 2) {
            Value nil = {VALUE_NIL, .length = 0};
            return nil;
          }
          
          Value* argv = malloc(2 * sizeof(Value));
          ValueList* current = value.as.list->next;
          
          // Don't evaluate the first argument (symbol)
          argv[0] = current->value; 
          current = current->next;
          
          // Evaluate the second argument
          argv[1] = eval_value(interp, current->value);
          
          Value result = builtin_set(argc, argv, interp);
          
          free(argv);
          
          return result;
        }
        
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
        	Value func  = env_lookup(interp->env, funcname);
	if(func.type != VALUE_CFUNC) {
		if (argv) free(argv);
		return (Value){.type = VALUE_NIL};
	}
	result = func.as.function(argc, argv);
        if (argv) {
          free(argv);
        }
        
        return result;
      }
  }
  
  Value nil = {VALUE_NIL, .length = 0};
  return nil;
}

Value eval_sexp(interpreter_t* interp, SExp* input) {
  if (!input) {
    Value nil = {VALUE_NIL, .length = 0};
    return nil;
  }
  
  Value value = sexp_to_value(input);
  Value result = eval_value(interp, value);
  return result;
}
