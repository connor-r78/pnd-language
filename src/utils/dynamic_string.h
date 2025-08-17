#ifndef PND_UTILS_STRING_H_
#define PND_UTILS_STRING_H_

#include <stddef.h>

// String.
// Dynamic string buffer allocated on heap.
typedef struct {
  char* data;

  size_t len;
  size_t capacity;
} String;

// Create new empty string.
// Panic on `malloc` NULL pointer.
String string_new();
// Create new string with provided input (copies value).
// Panic on NULL input pointer.
String string_from(const char* input);
// Create provided string's clone.
// Panic on empty input buffer.
String string_clone(String* other);

// Get character from string by position.
// Returns '\0' on position overflow and empty data.
char string_getc(String* buffer, size_t position);

// Replaces character on specified position and returns it.
// Returns '\0' on position overflow and empty data.
char string_setc(String* buffer, size_t position, char character);

// Get raw string pointer (heap pointer).
// Returns NULL on empty data.
char* string_get(String* buffer);

// Push new character to string.
// Automatically reallocates string if out of len bounds.
// Panic on `realloc` NULL pointer.
void string_push(String* buffer, char character);

// Push raw str to string.
// Copies value from provided pointer and places it at the end.
// Panic on `realloc` NULL pointer.
void string_push_str(String* buffer, char* input);

// Replaces data pointer with provided (NO COPY).
// Automatically frees previous data and calculates new len/capacity.
void string_set(String* buffer, char* pointer);

// Removes string buffer from memory.
void string_free(String* buffer);

#endif  // PND_UTILS_STRING_H_
