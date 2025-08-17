#include "dynamic_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

string_t string_new() {
  char* data = malloc(sizeof(char));
  size_t len = 0;
  size_t capacity = 1;

  if (!data) {
    fprintf(stderr, "Bad memory allocation: `string_push`");
    exit(1);
  }

  *data = '\0';

  return (string_t){.data = data, .len = len, .capacity = capacity};
}

string_t string_from(const char* input) {
  if (!input) {
    fprintf(stderr, "Null input pointer: `string_from`");
    exit(1);
  }

  char* data = strdup(input);
  size_t len = strlen(input);
  size_t capacity = len;

  return (string_t){.data = data, .len = len, .capacity = capacity};
}

string_t string_clone(string_t* other) {
  char* data = (other && other->data) ? strdup(other->data) : NULL;
  size_t len = (other) ? other->len : 0;
  size_t capacity = (other) ? other->capacity : 0;

  if (!data) {
    fprintf(stderr, "Clone of empty buffer: `string_clone`");
    exit(1);
  }

  return (string_t){.data = data, .len = len, .capacity = capacity};
}

char string_getc(string_t* buffer, size_t position) {
  if (!buffer || !buffer->data || position >= buffer->len || position <= 0)
    return '\0';

  return buffer->data[position];
}

char string_setc(string_t* buffer, size_t position, char character) {
  if (!buffer || !buffer->data || position >= buffer->len || position <= 0)
    return '\0';

  char previous = buffer->data[position];
  buffer->data[position] = character;

  return previous;
}

char* string_get(string_t* buffer) {
  if (!buffer || !buffer->data)
    return NULL;

  return buffer->data;
}

void string_push(string_t* buffer, char character) {
  buffer->len += 1;

  if (buffer->len > buffer->capacity) {
    char* new_ptr = realloc(buffer->data, sizeof(char) * buffer->len);

    if (!new_ptr) {
      fprintf(stderr, "Bad memory allocation: `string_push`");
      exit(1);
    }

    buffer->capacity = buffer->len;
    buffer->data = new_ptr;
  }

  buffer->data[buffer->len - 1] = character;
  buffer->data[buffer->len] = '\0';
}

void string_push_str(string_t* buffer, char* input) {
  size_t input_length = strlen(input);
  size_t previous_length = buffer->len;

  buffer->len += input_length;

  if (buffer->len > buffer->capacity) {
    char* new_ptr = realloc(buffer->data, sizeof(char) * buffer->len);

    if (!new_ptr) {
      fprintf(stderr, "Bad memory allocation: `string_push`");
      exit(1);
    }

    buffer->capacity = buffer->len;
    buffer->data = new_ptr;
  }

  char* cursor = buffer->data;
  cursor += previous_length;

  strcpy(cursor, input);
}

void string_set(string_t* buffer, char* pointer) {
  if (!buffer || !pointer)
    return;

  if (buffer->data)
    free(buffer->data);

  buffer->data = pointer;
  buffer->len = strlen(pointer);
  buffer->capacity = buffer->len;
}

void string_free(string_t* buffer) {
  if (!buffer)
    return;

  if (buffer->data)
    free(buffer->data);

  buffer->len = 0;
  buffer->capacity = 0;
}
