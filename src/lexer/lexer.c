#include "../lexer/lexer.h"
#include "../gc/gc.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tokens stringified variants
const char* kTokenNames[] = {"TOKEN_LPAREN", "TOKEN_RPAREN", "TOKEN_SYMBOL",
                             "TOKEN_NUMBER", "TOKEN_STRING", "TOKEN_QUOTE",
                             "TOKEN_ERROR",  "TOKEN_EOF"};

// prototypes
void tokenize_next(TokenStreamer* streamer);

// implementations

TokenStreamer token_streamer_init(const char* input) {
  TokenStreamer streamer;

  streamer.input = input;
  streamer.input_length = strlen(input);
  streamer.position = 0;

  streamer.current_line = 1;
  streamer.current_column = 1;

  streamer.current = NULL;
  streamer.previous = NULL;

  return streamer;
}

Token* token_streamer_current(TokenStreamer* streamer) {
  if (streamer && streamer->current) {
    return streamer->current;
  }
  return NULL;
}

Token* token_streamer_previous(TokenStreamer* streamer) {
  if (streamer && streamer->previous) {
    return streamer->previous;
  }
  return NULL;
}

Token* token_streamer_next(TokenStreamer* streamer) {
  if (!streamer)
    return NULL;

  // moving current to previous

  Token* previous = streamer->current;

  if (streamer->previous)
    token_free(streamer->previous);

  streamer->previous = previous;
  streamer->current = NULL;

  // tokenizing new

  tokenize_next(streamer);
  return token_streamer_current(streamer);
}

void token_streamer_free(TokenStreamer* streamer) {
  if (streamer) {
    if (streamer->current) {
      token_free(streamer->current);
      streamer->current = NULL;
    }

    if (streamer->previous) {
      token_free(streamer->previous);
      streamer->previous = NULL;
    }
  }
}

bool is_allowed_special_character(char c) {
  switch (c) {
    case '*':
    case '-':
    case '+':
    case '/':
    case '_':
      return true;
    default:
      return false;
  };
}

Token* tokengen(TokenType type, const char* value) {
  Token* ret = gc_alloc(sizeof(Token));

  if (!ret)
    return NULL;

  ret->type = type;
  ret->value = string_from(value);

  return ret;
}

void tokenize_next(TokenStreamer* streamer) {
  // Whitespace (skip)
  while (streamer->position < streamer->input_length &&
         isspace(streamer->input[streamer->position])) {
    if (streamer->input[streamer->position] == '\n') {
      streamer->current_line++;
      streamer->current_column = 1;
    } else {
      streamer->current_column++;
    }
    streamer->position++;
  }

  // EOF
  if (streamer->position >= streamer->input_length) {
    streamer->current = tokengen(TOKEN_EOF, "");
    return;
  }

  char current = streamer->input[streamer->position];

  // Left Paren
  if (current == '(') {
    streamer->current_column++;
    streamer->position++;
    streamer->current = tokengen(TOKEN_LPAREN, "(");
    return;
  }

  // Right Paren
  if (current == ')') {
    streamer->current_column++;
    streamer->position++;
    streamer->current = tokengen(TOKEN_RPAREN, ")");
    return;
  }

  // Quote
  if (current == '\'') {
    streamer->current_column++;
    streamer->position++;
    streamer->current = tokengen(TOKEN_QUOTE, "'");
    return;
  }

  // Number
  if (isdigit(current)) {
    String buffer = string_new();

    while (streamer->position < streamer->input_length &&
           isdigit(streamer->input[streamer->position])) {
      string_push(&buffer, streamer->input[streamer->position++]);
      streamer->current_column++;
    }

    streamer->current = tokengen(TOKEN_NUMBER, string_get(&buffer));
    string_free(&buffer);
    return;
  }

  // String
  if (current == '"') {
    streamer->position++;
    streamer->current_column++;

    String buffer;

    while (streamer->position < streamer->input_length &&
           streamer->input[streamer->position] != '"') {
      string_push(&buffer, streamer->input[streamer->position++]);
      streamer->current_column++;
    }

    if (streamer->position >= streamer->input_length) {
      streamer->current = tokengen(TOKEN_ERROR, "Unterminated string");
      string_free(&buffer);
      return;
    }

    if (streamer->input[streamer->position] == '"') {
      streamer->position++;
      streamer->current_column++;
    }

    streamer->current = tokengen(TOKEN_STRING, string_get(&buffer));
    string_free(&buffer);
    return;
  }

  // Special Chars
  String buffer = string_new();

  while (streamer->position < streamer->input_length &&
         (isalnum(streamer->input[streamer->position]) ||
          is_allowed_special_character(streamer->input[streamer->position]))) {
    string_push(&buffer, streamer->input[streamer->position++]);
    streamer->current_column++;
  }

  if (buffer.len < 1) {
    // No token was matched by previous rules
    char error_msg[50];
    snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c",
             streamer->input[streamer->position]);

    streamer->position++;  // Advance past the problematic character
    streamer->current_column++;

    streamer->current = tokengen(TOKEN_ERROR, error_msg);
    string_free(&buffer);
    return;
  }

  streamer->current = tokengen(TOKEN_SYMBOL, string_get(&buffer));
  string_free(&buffer);
}

void token_print(Token* token) {
  printf("TokenType: %s, value %s", kTokenNames[token->type],
         string_get(&token->value));
}

void token_free(Token* token) {
  if (token) {
    string_free(&token->value);
    free(token);
  }
}
