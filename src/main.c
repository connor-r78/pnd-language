#include "lex.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
void remove_outer_quotes(char *str) {
	size_t len = strlen(str);
	if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'') {
        memmove(str, str + 1, len - 2); 
        str[len - 2] = '\0'; 
    }
}
void parse_and_print(char* input) {
    init_lexer(input, strlen(input));
    Token* token = next_token();
    SExp* sexp = parse_sexp(token);
    print_sexp(sexp);
    printf("\n");

}

int main(int argc, char **argv) {
  
    int opt;
    bool parse = false;
	char* input = NULL;
    while((opt = getopt(argc, argv, "pc:")) != -1) 
    { 
        switch(opt) 
        { 
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

	if(!input) {
		fprintf(stderr, "Usage: %s -c 'code'\n", argv[0]);
		return 1;
	}
	remove_outer_quotes(input);
	if(parse) {
		parse_and_print(input);
	} else {
		init_lexer(input, strlen(input));
		//memory leak
		Token *token = next_token();
		while ((token->type != TOKEN_EOF)) {
			print_token(token);
			printf("\n");
			free_token(token);
			token = next_token();
		}
	}

  return 0;
}
