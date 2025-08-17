#ifndef PND_LEXER_H_
#define PND_LEXER_H_

#define MAX_NUM_SIZE 256
#define MAX_STR_SIZE 1024
#define MAX_SYM_SIZE 1024

#include <stddef.h>

#include "../utils/dynamic_string.h"

// Token.
// Abstract structure which contains type, value, and metadata of lexeme.
typedef enum {
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SYMBOL,
  TOKEN_NUMBER,
  TOKEN_STRING,
  TOKEN_ERROR,
  TOKEN_EOF
} token_type_t;

typedef struct {
  token_type_t type;
  string_t value;
} token_t;

// Token Streamer.
// Simple wrapper to keep current and previous tokens and automatically free
// them when moving through lexems.
typedef struct {
  const char* input;
  size_t input_length;

  size_t current_line;
  size_t current_column;
  size_t position;

  token_t* current;
  token_t* previous;
} token_streamer;

// Streamer Initialization
token_streamer token_streamer_init(const char* input);
// Get Current Token
token_t* token_streamer_current(token_streamer* streamer);
// Get Previous Token
token_t* token_streamer_previous(token_streamer* streamer);
// Tokenize Next Token
token_t* token_streamer_next(token_streamer* streamer);
// Free Streamer from memory (frees only content inside)
void token_streamer_free(token_streamer* streamer);

// -------------------------------------

void token_print(token_t* token);
void token_free(token_t* token);

#endif  // PND_LEXER_H
