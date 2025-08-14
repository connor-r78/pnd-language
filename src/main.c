#include "lex.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void remove_outer_quotes(char *str) {
	size_t len = strlen(str);
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
        memmove(str, str + 1, len - 2); 
        str[len - 2] = '\0'; 
    }
}
int main(int argc, char **argv) {
  
  char* input = argv[1];
  remove_outer_quotes(input);
  init_lexer(input, strlen(input));
  //memory leak
  Token *token = next_token();
  while ((token->type != TOKEN_EOF)) {
    print_token(token);
    printf("\n");
    free_token(token);
    token = next_token();
  }

  return 1;
}
