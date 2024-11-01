#include <stdio.h>
#include <string.h>

#include "database.h"
#include "menu.h"
#include "utils.h"

int login();
void display_menu();

int main() {
  // stop program for an invalid login
  if (login() == 0)
    return -1;

  // display the menu when successfully logged in
  display_menu();

  // stop at menu while enter key is not pressed
  printf("\n");
  printf("Press enter to continue...");
  while (getchar() != '\n')
    ;

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

void display_menu() {
  for (int item = 0; item < ITEMS; item++) {
    printf("%s\t%s\t%s\n", MENU[item][0], MENU[item][1], MENU[item][2]);
  }
}