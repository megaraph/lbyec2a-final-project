// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#include <stddef.h>

#include "utils.h"

char *str_input(char *prompt, char *buffer, size_t size) {
  printf("%s", prompt);
  fgets(buffer, size, stdin);

  // Remove the newline character at the end, if it exists
  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0'; // Replace newline with null terminator
  }

  return buffer; // Return the validbuffer
}

int find_account(const char *username,
                 const char database[DB_SIZE][2][ENCRYPT_SIZE]) {
  for (int row = 0; row < DB_SIZE; row++) {
    if (strcmp(username, database[row][0]) == 0)
      return row;
  }
  return -1;
}

int verify_pw(int user_index, const char *password,
              const char database[DB_SIZE][2][ENCRYPT_SIZE]) {

  if (strcmp(password, database[user_index][1]) == 0)
    return 1;

  return 0;
}

#endif // UTILS_H