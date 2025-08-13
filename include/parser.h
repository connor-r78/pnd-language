#ifndef _PANDORA_TYPES_H
#define _PANDORA_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *src;
  int length;
  int cursor;
} parser;

typedef struct {
  
} stack;

parser *parser_init(char *src, int length);
void parser_parse(parser* p);

#endif // _PANDORA_TYPES_H
