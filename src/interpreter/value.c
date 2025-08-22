#include "value.h"
#include "../gc/gc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void value_print(const Value* value) {
  if (!value) {
    printf("NULL");
    return;
  }

  switch (value->type) {
    case VALUE_NIL:
      printf("nil");
      break;
    case VALUE_NUMBER:
      printf("%.6g", value->as.number);
      break;
    case VALUE_STRING:
      printf("\"%s\"", value->as.string);
      break;
    case VALUE_SYMBOL:
      printf("%s", value->as.symbol);
      break;
    case VALUE_LIST:
      printf("(");
      ValueList* current = value->as.list;
      while (current) {
        value_print(&current->value);
        if (current->next) {
          printf(" ");
        }
        current = current->next;
      }
      printf(")");
      break;
    case VALUE_CFUNC:
      printf("<c function>");
      break;
    case VALUE_LAMBDA:
      // TODO: actually print
      printf("<lambda function>");
  }
}

Value sexp_to_value(const SExp* sexp) {
  if (!sexp) {
    Value nil = {VALUE_NIL, .length = 0};
    return nil;
  }

  Value val;
  val.length = 0;

  switch (sexp->type) {
    case SEXP_NIL:
      val.type = VALUE_NIL;
      break;
    case SEXP_NUMBER:
      val.type = VALUE_NUMBER;
      val.as.number = sexp->as.number;
      break;
    case SEXP_STRING:
      val.type = VALUE_STRING;
      val.as.string = strdup(sexp->as.string);
      break;
    case SEXP_SYMBOL:
      val.type = VALUE_SYMBOL;
      val.as.symbol = strdup(sexp->as.symbol);
      break;
    case SEXP_LIST: {
      val.type = VALUE_LIST;
      val.as.list = NULL;
      val.length = 0;

      SExpList* current = sexp->as.list;
      ValueList** list_tail = &val.as.list;

      while (current) {
        ValueList* new_node = gc_alloc(sizeof(ValueList));
        new_node->value = sexp_to_value(current->value);
        new_node->next = NULL;

        *list_tail = new_node;
        list_tail = &new_node->next;
        val.length++;

        current = current->next;
      }
    } break;
  }
  return val;
}

// leak memory for now
void value_free(Value* val) {
  if (val->type == VALUE_LIST) {
    ValueList* current = val->as.list;
    ValueList* next;
    while (current) {
      next = current->next;
      value_free(&current->value);
      free(current);
      current = next;
    }
  }
  // dont since they are on the stack for now
  // free(val);
}
