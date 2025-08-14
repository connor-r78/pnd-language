#ifndef LEX_H
#define LEX_H



#define MAX_NUM_SIZE 256
#define MAX_STR_SIZE 1024
#define MAX_SYM_SIZE 1024

typedef enum {
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SYMBOL,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_SYM_SIZE];
    int length;
} Token;

void init_lexer(const char* input, int length);
Token* next_token();
void print_token(Token *tok);
void free_token(Token* tok);

#endif
