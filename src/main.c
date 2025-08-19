#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interpreter/env.h"
#include "interpreter/interpreter.h"
#include "interpreter/value.h"
#include "lexer/lexer.h"
#include "parser/parse.h"

void remove_outer_quotes(char* str) {
  size_t len = strlen(str);

  if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';
  }
}

void print_usage(char* name) {
  fprintf(stderr,
          "Usage: %s [options] <filename>\n"
          "Options:"
          "-t   Print tokens."
          "-p   Print AST.",
          name);
}

int main(int argc, char** argv) {
  int opt;
  bool parse = false;
  bool tokenize = false;
  bool execute = true;

  while ((opt = getopt(argc, argv, "tp")) != -1) {
    switch (opt) {
      case 't':
        tokenize = true;
        execute = false;
        break;
      case 'p':
        parse = true;
        execute = false;
        break;
      case '?':
        fprintf(stderr, "unknown option: %c\n", optopt);
        print_usage(argv[0]);
        break;
    }
  }

  char* input = argv[optind];
  if (!input) {
    fprintf(stderr, "Usage: %s -c 'code'\n", argv[0]);
    print_usage(argv[0]);
    return 1;
  }

  remove_outer_quotes(input);

  token_streamer streamer = token_streamer_init(input);
  if (execute) {
    interpreter_t* interp = init_interpreter();

    while (1) {
      token_t* token = token_streamer_next(&streamer);
      if (token->type == TOKEN_EOF) {
        break;
      }
      SExp* sexp = parse_sexp(&streamer, token);
      if (!sexp) {
        break;
      }
      Value result = eval_sexp(interp, sexp);
      if (result.type != VALUE_NIL) {
        value_print(&result);
        printf("\n");
      }
      // No need to free until we have GC
    }

    interpreter_free(interp);
    token_streamer_free(&streamer);
  }

  if (tokenize) {
    token_streamer streamer = token_streamer_init(input);

    token_t* token = token_streamer_next(&streamer);

    while ((token->type != TOKEN_EOF)) {
      token_print(token);
      printf(" ");
      fflush(stdout);
      token = token_streamer_next(&streamer);
    }
    printf("\n");
    token_streamer_free(&streamer);
  }
  if (parse) {
    streamer = token_streamer_init(input);

    while (1) {
      token_t* token = token_streamer_next(&streamer);
      if (token->type == TOKEN_EOF) {
        break;
      }
      SExp* sexp = parse_sexp(&streamer, token);
      if (!sexp) {
        break;
      }
      print_sexp(sexp);
      printf("\n");
    }
    token_streamer_free(&streamer);
  }

  return 0;
}
