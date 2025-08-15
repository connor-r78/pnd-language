#include "../lexer/lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tokens stringified variants
const char* kTokenNames[] = {"TOKEN_LPAREN", "TOKEN_RPAREN", "TOKEN_SYMBOL",
                             "TOKEN_NUMBER", "TOKEN_STRING", "TOKEN_ERROR",
                             "TOKEN_EOF"};

// prototypes
void tokenize_next(token_streamer* streamer);

// implementations

token_streamer token_streamer_init(const char* input) {
  token_streamer streamer;

  streamer.input = input;
  streamer.input_length = strlen(input);

  streamer.current_line = 0;
  streamer.current_column = 0;

  streamer.current = NULL;
  streamer.previous = NULL;

  return streamer;
}

token_t* token_streamer_current(token_streamer* streamer) {
  if (streamer && streamer->current) {
    return streamer->current;
  }
  return NULL;
}

token_t* token_streamer_previous(token_streamer* streamer) {
  if (streamer && streamer->previous) {
    return streamer->previous;
  }
  return NULL;
}

token_t* token_streamer_next(token_streamer* streamer) {
  if (!streamer)
    return NULL;

  // moving current to previous

  token_t* previous = streamer->current;

  if (streamer->previous)
    token_free(streamer->previous);

  streamer->previous = previous;

  // tokenizing new

  tokenize_next(streamer);
  return token_streamer_current(streamer);
}

void token_streamer_free(token_streamer* streamer) {
  if (streamer) {
    if (streamer->current)
      token_free(streamer->current);
    if (streamer->previous)
      token_free(streamer->previous);
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

token_t* tokengen(token_type_t type, char* val) {
  token_t* ret = malloc(sizeof(token_t));

  ret->type = type;

  strncpy(ret->value, val, MAX_SYM_SIZE - 1);

  ret->value[MAX_SYM_SIZE - 1] = '\0';
  ret->length = (int)strlen(ret->value);

  return ret;
}

void tokenize_next(token_streamer* streamer) {
  const char* kInputStartPtr = streamer->input;
  const char* kCurrentInputPtr = streamer->input;

  while (*kCurrentInputPtr != '\0' && isspace(*kCurrentInputPtr)) {
    if (*kCurrentInputPtr == '\n') {
      streamer->current_line++;
      streamer->current_column = 1;
    } else {
      streamer->current_column++;
    }

    kCurrentInputPtr++;
  }

  char current = *kCurrentInputPtr;

  if (current == '\0') {
    streamer->current = tokengen(TOKEN_EOF, "");
    return;
  }

  if (current == '(') {
    streamer->current_column++;
    streamer->current = tokengen(TOKEN_LPAREN, "(");
    kCurrentInputPtr++;
    return;
  }

  if (current == ')') {
    streamer->current_column++;
    streamer->current = tokengen(TOKEN_RPAREN, ")");
    kCurrentInputPtr++;
    return;
  }

  if (isdigit(current)) {
    char buffer[MAX_NUM_SIZE];
    int i = 0;

    while (isdigit(*kCurrentInputPtr) && i < MAX_NUM_SIZE - 1 &&
           (kCurrentInputPtr - kInputStartPtr < streamer->input_length)) {
      buffer[i++] = *kCurrentInputPtr++;
      streamer->current_column++;
    }

    if (i >= MAX_NUM_SIZE - 1) {
      streamer->current = tokengen(TOKEN_ERROR, "Number too long");
      return;
    }

    buffer[i] = '\0';
    streamer->current = tokengen(TOKEN_NUMBER, buffer);
    return;
  }

  if (current == '"') {
    kCurrentInputPtr++;
    streamer->current_column++;

    char buffer[MAX_STR_SIZE];
    int i = 0;

    while (*kCurrentInputPtr != '"' && i < MAX_STR_SIZE - 1 &&
           (kCurrentInputPtr - kInputStartPtr < streamer->input_length)) {
      buffer[i++] = *kCurrentInputPtr++;
      streamer->current_column++;
    }

    if (i >= MAX_STR_SIZE - 1) {
      streamer->current = tokengen(TOKEN_ERROR, "String too long");
      return;
    }

    if (*kCurrentInputPtr == '\0') {
      streamer->current = tokengen(TOKEN_ERROR, "Unterminated string");
      return;
    }

    buffer[i] = '\0';

    if (*kCurrentInputPtr == '"') {
      kCurrentInputPtr++;
      streamer->current_column++;
    }

    streamer->current = tokengen(TOKEN_STRING, buffer);
    return;
  }

  char buffer[MAX_SYM_SIZE];
  size_t i = 0;

  while ((isalnum(*kCurrentInputPtr) ||
          is_allowed_special_character(*kCurrentInputPtr)) &&
         i < MAX_SYM_SIZE - 1 &&
         (kCurrentInputPtr - kInputStartPtr < streamer->input_length)) {
    buffer[i++] = *kCurrentInputPtr++;
    streamer->current_column++;
  }

  if (i >= MAX_SYM_SIZE - 1) {
    streamer->current = tokengen(TOKEN_ERROR, "Symbol too long");
    return;
  }

  buffer[i] = '\0';

  if (i == 0) {
    // No token was matched by previous rules
    char error_msg[50];
    snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c",
             *kCurrentInputPtr);

    kCurrentInputPtr++;  // Advance past the problematic character
    streamer->current_column++;

    tokengen(TOKEN_ERROR, error_msg);
  }

  tokengen(TOKEN_SYMBOL, buffer);
}

void token_print(token_t* token) {
  printf("TokenType: %s, value %s", kTokenNames[token->type], token->value);
}

void token_free(token_t* token) {
  if (token)
    free(token);
}
