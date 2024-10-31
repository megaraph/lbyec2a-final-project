#include <stdio.h>
#include <string.h>

#include "utils.h"

#define ENCRYPT_SIZE 64

int login() {
  char username_buffer[ENCRYPT_SIZE], pw_buffer[ENCRYPT_SIZE];

  const char *username = str_input("USERNAME: ", username_buffer, ENCRYPT_SIZE);
  const char *password = str_input("PASSWORD: ", pw_buffer, ENCRYPT_SIZE);

  printf("Username: %s\n", username);
  printf("Password: %s\n", password);

  return 0;
}

int main() {
  login();
  return 0;
}