#include "parser.h"

char peek(parser *p);
parser *advance(parser *p);
parser *skip_ws(parser *p);

parser *parser_init(char *src, int length) {
  parser *p;

  p->length = length;
  p->cursor = 0;
  p->src = (char *)malloc((length + 1) * sizeof(char));

  if (p->src != NULL) {
    strncpy(p->src, src, length);
    p->src[length] = '\0';
  }
  return p;
}

void parser_parse(parser *p) {
  while (p->cursor < p->length) {
    char curr_char = peek(p);
    switch (curr_char) {
    case '\"':
      handle_string(p);
      advance(p);
      break;

    case ' ':
    case '\t':
    case '\n':
    case '\r':
      skip_ws(p);
      advance(p);
      break;

    default:
      break;
    }
  }
}

char peek(parser *p) { return p->src[p->cursor]; }

parser *advance(parser *p) {
  p->cursor++;
  return p;
}

parser *skip_ws(parser *p) {
  while (peek(p) == ' ' || peek(p) == '\t' || peek(p) == '\n' ||
         peek(p) == '\r') {
    advance(p);
  }
  return p;
}

parser *handle_string(parser *p) {
  int start = p->cursor; // string start pos
  advance(p);            // advance to go to next char after double quote
  while (peek(p) != '\"') {
    if (peek(p) == "\\") {
      advance(p);
      advance(p);
      // advancing 2 times in row since we want to escape this character
    }
    advance(p);
  }

  int end = p->cursor;
   // +1 for null termination character
  char *string = (char *)malloc((end - start + 1) * sizeof(char));
  
}