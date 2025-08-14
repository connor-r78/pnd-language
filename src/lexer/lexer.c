#include "../lexer/lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* kCurrentInputPtr;
static const char* kInputStartPtr;

static int current_line;
static int current_column;

int len;

// Tokens stringified variants
const char* kTokenNames[] = {"TOKEN_LPAREN", "TOKEN_RPAREN", "TOKEN_SYMBOL",
                             "TOKEN_NUMBER", "TOKEN_STRING", "TOKEN_ERROR",
                             "TOKEN_EOF"};

void init_lexer(const char* input, int length) {
  kCurrentInputPtr = input;
  kInputStartPtr = input;

  current_line = 1;
  current_column = 1;

  len = length;
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

Token* tokengen(TokenType type, char* val) {
  Token* ret = malloc(sizeof(Token));

  ret->type = type;

  strncpy(ret->value, val, MAX_SYM_SIZE - 1);

  ret->value[MAX_SYM_SIZE - 1] = '\0';
  ret->length = (int)strlen(ret->value);

  return ret;
}

Token* next_token() {
  while (*kCurrentInputPtr != '\0' && isspace(*kCurrentInputPtr)) {
    if (*kCurrentInputPtr == '\n') {
      current_line++;
      current_column = 1;
    } else {
      current_column++;
    }

    kCurrentInputPtr++;
  }

  char current = *kCurrentInputPtr;

  if (current == '\0') {
    return tokengen(TOKEN_EOF, "");
  }

  if (current == '(') {
    current_column++;
    kCurrentInputPtr++;
    return tokengen(TOKEN_LPAREN, "(");
  }

  if (current == ')') {
    current_column++;
    kCurrentInputPtr++;
    return tokengen(TOKEN_RPAREN, ")");
  }

  if (isdigit(current)) {
    char buffer[MAX_NUM_SIZE];
    int i = 0;

    while (isdigit(*kCurrentInputPtr) && i < MAX_NUM_SIZE - 1 &&
           (kCurrentInputPtr - kInputStartPtr < len)) {
      buffer[i++] = *kCurrentInputPtr++;
      current_column++;
    }

    if (i >= MAX_NUM_SIZE - 1) {
      return tokengen(TOKEN_ERROR, "Number too long");
    }

    buffer[i] = '\0';
    return tokengen(TOKEN_NUMBER, buffer);
  }

  if (current == '"') {
    kCurrentInputPtr++;
    current_column++;

    char buffer[MAX_STR_SIZE];
    int i = 0;

    while (*kCurrentInputPtr != '"' && i < MAX_STR_SIZE - 1 &&
           (kCurrentInputPtr - kInputStartPtr < len)) {
      buffer[i++] = *kCurrentInputPtr++;
      current_column++;
    }

    if (i >= MAX_STR_SIZE - 1) {
      return tokengen(TOKEN_ERROR, "String too long");
    }

    if (*kCurrentInputPtr == '\0') {
      return tokengen(TOKEN_ERROR, "Unterminated string");
    }

    buffer[i] = '\0';

    if (*kCurrentInputPtr == '"') {
      kCurrentInputPtr++;
      current_column++;
    }

    return tokengen(TOKEN_STRING, buffer);
  }

  char buffer[MAX_SYM_SIZE];
  size_t i = 0;

  while ((isalnum(*kCurrentInputPtr) ||
          is_allowed_special_character(*kCurrentInputPtr)) &&
         i < MAX_SYM_SIZE - 1 && (kCurrentInputPtr - kInputStartPtr < len)) {
    buffer[i++] = *kCurrentInputPtr++;
    current_column++;
  }

  if (i >= MAX_SYM_SIZE - 1) {
    return tokengen(TOKEN_ERROR, "Symbol too long");
  }

  buffer[i] = '\0';

  if (i == 0) {
    // No token was matched by previous rules
    char error_msg[50];
    snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c",
             *kCurrentInputPtr);

    kCurrentInputPtr++;  // Advance past the problematic character
    current_column++;

    return tokengen(TOKEN_ERROR, error_msg);
  }

  return tokengen(TOKEN_SYMBOL, buffer);
}

void print_token(Token* tok) {
  printf("TokenType: %s, value %s", kTokenNames[tok->type], tok->value);
}

void free_token(Token* tok) {
  if (tok) {
    free(tok);
  }
}
