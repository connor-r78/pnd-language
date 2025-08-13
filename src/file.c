#include "file.h"

char *read_file(char *filename) {
  FILE *f = fopen(filename, "rb");

  if (f == NULL) {
    printf("ERR: could not open file: %s\n", strerror(errno));
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  int length = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *buffer = (char *)malloc(length);
  if (buffer) {
    fread(buffer, 1, length, f);
  }

  return buffer;

  fclose(f);
}