#ifndef PND_VALUE_H
#define PND_VALUE_H

#include "../parser/parse.h"

#include <stddef.h>
#include <stdbool.h>

typedef enum {
  VALUE_NIL,
  VALUE_NUMBER,
  VALUE_STRING,
  VALUE_SYMBOL,
  VALUE_LIST,
  VALUE_CFUNC,
} ValueType;

// Forward declarations
typedef struct Value Value;
typedef struct ValueList ValueList;
typedef Value (*BuiltinFunction)(size_t argc, Value* argv);

struct Value {
  ValueType type;
  union {
    double number;
    char* string;
    char* symbol;
    ValueList* list;
    BuiltinFunction function;
  } as;
	//TODO: move this
  size_t length; // for lists, 0 for non-lists
};

struct ValueList {
  Value value;
  struct ValueList* next;
};

void value_print(const Value* value);
Value sexp_to_value(const SExp* sexp);
//leaks memory untill garbage collector
void value_free(Value* val);

#endif // PND_VALUE_H
