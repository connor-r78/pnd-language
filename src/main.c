#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "interpreter/env.h"
#include "interpreter/interpreter.h"
#include "lexer/lexer.h"
#include "parser/parse.h"

void remove_outer_quotes(char* str) {
  size_t len = strlen(str);

  if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';
  }
}

int main(int argc, char** argv) {
  int opt;
  bool parse = false;
  char* input = NULL;

  while ((opt = getopt(argc, argv, "pc:")) != -1) {
    switch (opt) {
      case 'p':
        parse = true;
        break;
      case 'c':
        input = optarg;
        break;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
    }
  }

  if (!input) {
    fprintf(stderr, "Usage: %s -c 'code'\n", argv[0]);
    return 1;
  }

  remove_outer_quotes(input);

  if (parse) {
    token_streamer streamer = token_streamer_init(input);
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
      SExp* result = eval_sexp(interp, sexp);
      if (result) {
        print_sexp(result);
        printf("\n");
      }
    }

    token_streamer_free(&streamer);
    interpreter_free(interp);

  } else {
    token_streamer streamer = token_streamer_init(input);

    token_t* token = token_streamer_next(&streamer);

    while ((token->type != TOKEN_EOF)) {
      token_print(token);
      printf("\n");
      token = token_streamer_next(&streamer);
    }

    token_streamer_free(&streamer);
  }

  return 0;
}
