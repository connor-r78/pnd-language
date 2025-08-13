#include <stdio.h>

#include "file.h"

int main() {

  printf("%s\n", read_file("idea.clj"));

  return 0;
}
