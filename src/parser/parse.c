#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lexer/lexer.h"

void print_sexp(SExp* sexp) {
  if (sexp == NULL) {
    printf("NULL");
    return;
  }

  switch (sexp->type) {
    case SEXP_SYMBOL:
      printf("%s", sexp->as.symbol);
      break;
    case SEXP_NUMBER:
      printf("%f", sexp->as.number);
      break;
    case SEXP_STRING:
      printf("\"%s\"", sexp->as.string);
      break;
    case SEXP_LIST:
      printf("(");
      SExpList* list = sexp->as.list;
      while (list != NULL) {
        print_sexp(list->value);
        if (list->next != NULL) {
          printf(" ");
        }
        list = list->next;
      }
      printf(")");
      break;
  }
}

SExp* parse_list() {
  SExp* ret = malloc(sizeof(SExp));
  ret->type = SEXP_LIST;
  ret->as.list = NULL;

  SExpList* head = NULL;
  SExpList* tail = NULL;

  size_t len = 0;

  Token* token = next_token();
  while (token->type != TOKEN_RPAREN && token->type != TOKEN_EOF) {
    SExp* elem = parse_sexp(token);
    if (elem) {
      if (head == NULL) {
        head = malloc(sizeof(SExpList));
        head->value = elem;
        head->next = NULL;
        tail = head;
      } else {
        tail->next = malloc(sizeof(SExpList));
        tail = tail->next;
        tail->value = elem;
        tail->next = NULL;
      }
    }
    len++;
    token = next_token();
  }

  ret->as.list = head;
  ret->length = len;
  return ret;
}

SExp* parse_sexp(Token* token) {
  SExp* ret = malloc(sizeof(SExp));
  if (!ret) {
    return NULL;
  }

  switch (token->type) {
    case TOKEN_LPAREN:
      free(ret);
      return parse_list();
    case TOKEN_SYMBOL:
      ret->type = SEXP_SYMBOL;
      ret->as.symbol = strdup(token->value);
      return ret;
    case TOKEN_NUMBER:
      ret->type = SEXP_NUMBER;
      ret->as.number = atof(token->value);
      return ret;
    case TOKEN_STRING:
      ret->type = SEXP_STRING;
      ret->as.string = strdup(token->value);
      return ret;
    case TOKEN_RPAREN:
      free(ret);
      return NULL;
    default:
      free(ret);
      return NULL;
  }
}
