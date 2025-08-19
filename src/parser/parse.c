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
    case SEXP_NIL:
      printf("NIL");
      break;
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

static SExp* make_list_from_items(SExp** items, size_t count) {
  SExp* list = malloc(sizeof(SExp));
  list->type = SEXP_LIST;
  list->as.list = NULL;
  list->length = count;

  if (count == 0) {
    list->as.list = NULL;
    return list;
  }

  SExpList* head = NULL;
  SExpList* tail = NULL;
  for (size_t i = 0; i < count; i++) {
    SExpList* node = malloc(sizeof(SExpList));
    node->value = items[i];
    node->next = NULL;
    if (!head) {
      head = node;
      tail = node;
    } else {
      tail->next = node;
      tail = node;
    }
  }
  list->as.list = head;
  return list;
}

SExp* parse_list(token_streamer* streamer) {
  SExp* ret = malloc(sizeof(SExp));

  ret->type = SEXP_LIST;
  ret->as.list = NULL;

  SExpList* head = NULL;
  SExpList* tail = NULL;

  token_t* token = token_streamer_next(streamer);
  size_t len = 0;

  while (token->type != TOKEN_RPAREN && token->type != TOKEN_EOF) {
    SExp* elem = parse_sexp(streamer, token);

    if (elem) {
      if (head == NULL) {
        head = malloc(sizeof(SExpList));
        head->value = elem;
        head->next = NULL;
        tail = head;
      } else {
        SExpList* new_node = malloc(sizeof(SExpList));
        new_node->value = elem;
        new_node->next = NULL;
        tail->next = new_node;
        tail = new_node;
      }
    }

    len++;
    token = token_streamer_next(streamer);
  }

  ret->as.list = head;
  ret->length = len;

  return ret;
}

SExp* parse_sexp(token_streamer* streamer, token_t* token) {
  SExp* ret = malloc(sizeof(SExp));

  if (!ret) {
    return NULL;
  }

  switch (token->type) {
    case TOKEN_LPAREN:
      free(ret);
      return parse_list(streamer);
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
    case TOKEN_QUOTE: {
      token_t* next = token_streamer_next(streamer);
      SExp* quoted = parse_sexp(streamer, next);

      SExp* sym = malloc(sizeof(SExp));
      sym->type = SEXP_SYMBOL;
      sym->as.symbol = strdup("quote");

      if (quoted) {
        SExp* items[2];
        items[0] = sym;
        items[1] = quoted;
        SExp* list = make_list_from_items(items, 2);
        free(ret);
        return list;
      } else {
        SExp* items[1];
        items[0] = sym;
        SExp* list = make_list_from_items(items, 1);
        free(ret);
        return list;
      }
    }
    case TOKEN_RPAREN:
      free(ret);
      return NULL;
    default:
      free(ret);
      return NULL;
  }
}
