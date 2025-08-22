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
#include "gc/gc.h"

void remove_outer_quotes(char* str) {
  size_t len = strlen(str);

  if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
    memmove(str, str + 1, len - 2);
    str[len - 2] = '\0';
  }
}

void parse_and_print(char* input) {
  TokenStreamer streamer = token_streamer_init(input);

  Token* token = token_streamer_next(&streamer);
  SExp* sexp = parse_sexp(&streamer, token);

  print_sexp(sexp);
  printf("\n");
}

char* read_file(const char* filename) {
    FILE* f = fopen(filename, "rb");

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = gc_alloc(length + 1);

    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);

    return buffer;
}

void print_usage(char* name) {
  fprintf(stderr,
          "Usage: %s [options] <code>\n\n"
          "Options:\n"
          "-f <file> Read code from file\n"
          "-t        Print tokens.\n"
          "-p        Print AST.\n",
          name);
}

int main(int argc, char** argv) {
  int opt;
  bool parse = false;
  bool tokenize = false;
  bool execute = true;
  char* filename = NULL;

  while ((opt = getopt(argc, argv, "f:tp")) != -1) {
    switch (opt) {
      case 'f':
        filename = optarg;
        break;
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

  char* input = NULL;

  if (!filename) {
    input = argv[optind];
    if (!input) {
      print_usage(argv[0]);
      return 1;
    }
  } else {
    input = read_file(filename);
  }

  remove_outer_quotes(input);

  TokenStreamer streamer = token_streamer_init(input);
  if (execute) {
    interpreter_t* interp = init_interpreter();

    while (1) {
      Token* token = token_streamer_next(&streamer);
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
    TokenStreamer streamer = token_streamer_init(input);

    Token* token = token_streamer_next(&streamer);

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
      Token* token = token_streamer_next(&streamer);
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
