#include <stdio.h>
#include <string.h>

#include "database.h"
#include "menu.h"
#include "utils.h"

int login();

int main() {
  if (login() == 0)
    return -1;

  printf("Successful login.\n");

  return 0;
}

int login() {
  char username_buffer[ENCRYPT_SIZE], pw_buffer[ENCRYPT_SIZE];

  const char *username = str_input("USERNAME: ", username_buffer, ENCRYPT_SIZE);
  if (username[0] == '\0') {
    printf("Invalid username.\n");
    return 0;
  }

  const char *password = str_input("PASSWORD: ", pw_buffer, ENCRYPT_SIZE);
  if (password[0] == '\0') {
    printf("Invalid password.\n");
    return 0;
  }

  // check if username exists in database
  int user_index = find_account(username, DATABASE);

  if (user_index == -1) {
    printf("No account registered in system.\n");
    return 0;
  }

  if (!verify_pw(user_index, password, DATABASE)) {
    printf("Invalid password.\n");
    return 0;
  }

  return 1;
}