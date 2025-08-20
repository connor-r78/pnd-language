#ifndef PND_LEXER_H_
#define PND_LEXER_H_

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
  TOKEN_QUOTE,
  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  String value;
} Token;

// Token Streamer.
// Simple wrapper to keep current and previous tokens and automatically free
// them when moving through lexems.
typedef struct {
  const char* input;
  size_t input_length;

  size_t current_line;
  size_t current_column;
  size_t position;

  Token* current;
  Token* previous;
} TokenStreamer;

// Streamer Initialization
TokenStreamer token_streamer_init(const char* input);
// Get Current Token
Token* token_streamer_current(TokenStreamer* streamer);
// Get Previous Token
Token* token_streamer_previous(TokenStreamer* streamer);
// Tokenize Next Token
Token* token_streamer_next(TokenStreamer* streamer);
// Free Streamer from memory (frees only content inside)
void token_streamer_free(TokenStreamer* streamer);

// -------------------------------------

void token_print(Token* token);
void token_free(Token* token);

#endif  // PND_LEXER_H
