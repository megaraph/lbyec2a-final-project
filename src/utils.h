// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#include <stddef.h>

char *str_input(char *prompt, char *buffer, size_t size) {
  printf("%s", prompt);
  fgets(buffer, size, stdin);

  // Remove the newline character at the end, if it exists
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0'; // Replace newline with null terminator
  }

  // Check if the buffer is empty
  if (buffer[0] == '\0') {
    return ""; // Return an empty string if invalid
  }

  return buffer; // Return the validbuffer
}

#endif // UTILS_H