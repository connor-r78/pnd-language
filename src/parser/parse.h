#ifndef PND_PARSER_H_
#define PND_PARSER_H_

#include "../lexer/lexer.h"

typedef struct SExp SExp;

// SExp Atom | List<Sexp>
typedef enum {

  SEXP_SYMBOL,
  SEXP_NUMBER,
  SEXP_STRING,
  SEXP_LIST
} SExpType;

typedef struct SExpList {
  SExp* value;
  struct SExpList* next;
} SExpList;

struct SExp {
  SExpType type;
  union {
    char* symbol;
    double number;
    char* string;
    SExpList* list;
  } as;
  // store length in this struct, can move or change later if inefficent
  size_t length;
};

SExp* parse_sexp(token_streamer* streamer, token_t* token);

void print_sexp(SExp* sexp);
void parse_and_print(char* input);
void to_shell(SExp* input);

#endif  // PND_PARSER_H_
