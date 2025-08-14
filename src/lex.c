#include "lex.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
static const char *current_input_ptr;
static const char *input_start_ptr;
static int current_line;
static int current_column;
int len;
//for printing the enum
const char *tokenNames[] = {
   "TOKEN_LPAREN",
   "TOKEN_RPAREN",
   "TOKEN_SYMBOL",
   "TOKEN_NUMBER",
   "TOKEN_STRING",
   "TOKEN_ERROR",
   "TOKEN_EOF"
	};
void init_lexer(const char *input, int length) {
  current_input_ptr = input;
  input_start_ptr = input;
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

Token *tokengen(TokenType type, char *val) {
  Token *ret = malloc(sizeof(Token));
  ret->type = type;
  strncpy(ret->value, val, MAX_SYM_SIZE - 1);
  ret->value[MAX_SYM_SIZE - 1] = '\0';
  ret->length = strlen(ret->value);
  return ret;
}
Token *next_token() {
  while (*current_input_ptr != '\0' && isspace(*current_input_ptr)) {
    if (*current_input_ptr == '\n') {
      current_line++;
      current_column = 1;
    } else {
      current_column++;
    }
    current_input_ptr++;
  }
  char c = *current_input_ptr;
  if (c == '\0') {
    return tokengen(TOKEN_EOF, "");
  }
  if (c == '(') {
    current_column++;
    current_input_ptr++;
    return tokengen(TOKEN_LPAREN, "(");
  }
  if (c == ')') {
    current_column++;
    current_input_ptr++;
    return tokengen(TOKEN_RPAREN, ")");
  }
  if (isdigit(c)) {
    char buffer[MAX_NUM_SIZE];
    int i = 0;
    while (isdigit(*current_input_ptr) && i < MAX_NUM_SIZE - 1 &&
           (current_input_ptr - input_start_ptr < len)) {
      buffer[i++] = *current_input_ptr++;
      current_column++;
    }
    if (i >= MAX_NUM_SIZE - 1) {
        return tokengen(TOKEN_ERROR, "Number too long");
    }
    buffer[i] = '\0';
    return tokengen(TOKEN_NUMBER, buffer);
  }
  if (c == '"') {
    current_input_ptr++;
    current_column++;
    char buffer[MAX_STR_SIZE];
    int i = 0;
    while (*current_input_ptr != '"' && i < MAX_STR_SIZE - 1 &&
           (current_input_ptr - input_start_ptr < len)) {
      buffer[i++] = *current_input_ptr++;
      current_column++;
    }

    if (i >= MAX_STR_SIZE - 1) {
        return tokengen(TOKEN_ERROR, "String too long");
    }

    if (*current_input_ptr == '\0') {
        return tokengen(TOKEN_ERROR, "Unterminated string");
    }

    buffer[i] = '\0';
    if (*current_input_ptr == '"') {
      current_input_ptr++;
      current_column++;
    }
    return tokengen(TOKEN_STRING, buffer);
  }

  char buffer[MAX_SYM_SIZE];
  size_t i = 0;
  while ((isalnum(*current_input_ptr) || is_allowed_special_character(*current_input_ptr)) && i < MAX_SYM_SIZE - 1 &&
         (current_input_ptr - input_start_ptr < len)) {
    buffer[i++] = *current_input_ptr++;
    current_column++;
  }
  if (i >= MAX_SYM_SIZE - 1) {
      return tokengen(TOKEN_ERROR, "Symbol too long");
  }
  buffer[i] = '\0';
  if (i == 0) { // No token was matched by previous rules
      char error_msg[50];
      snprintf(error_msg, sizeof(error_msg), "Unexpected character: %c", *current_input_ptr);
      current_input_ptr++; // Advance past the problematic character
      current_column++;
      return tokengen(TOKEN_ERROR, error_msg);
  }
  return tokengen(TOKEN_SYMBOL, buffer);
}
void print_token(Token *tok) {
  printf("TokenType: %s, value %s", tokenNames[tok->type], tok->value);
}

void free_token(Token* tok) {
    if (tok) {
        free(tok);
    }
}
