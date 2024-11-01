// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stddef.h>

#include "config.h"

#ifdef _WIN32
#include <conio.h> // For getch() on Windows
#else
#include <termios.h> // For terminal control on UNIX/Linux
#include <unistd.h>  // For read() on UNIX/Linux
#endif

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

char *get_password(const char *prompt) {
  char *password = malloc(ENCRYPT_SIZE);
  if (password == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
  }

  int index = 0;
  char ch;

#ifdef _WIN32
  printf("%s", prompt);
  while (1) {
    ch = _getch();    // Get a character without echoing
    if (ch == '\r') { // Enter key
      break;
    } else if (ch == '\b') { // Backspace
      if (index > 0) {
        index--;
        printf("\b \b"); // Move back, print space, move back again
      }
    } else if (index < ENCRYPT_SIZE - 1) {
      password[index++] = ch;
      printf("*"); // Mask the character
    }
  }
#else
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ECHO); // Disable echo
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  printf("%s", prompt);
  while (1) {
    ch = getchar();
    if (ch == '\n' || ch == '\r') { // Enter key
      break;
    } else if (ch == 127 || ch == '\b') { // Backspace
      if (index > 0) {
        index--;
        printf("\b \b"); // Move back, print space, move back again
      }
    } else if (index < ENCRYPT_SIZE - 1) {
      password[index++] = ch;
      printf("*"); // Mask the character
    }
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old settings
#endif

  password[index] = '\0'; // Null-terminate the string
  printf("\n");           // Move to the next line
  return password;
}

int int_input_lower(char *prompt, char *error, int lower) {
  int num_input = lower - 1;
  do {
    printf("%s", prompt);
    int q_result = scanf("%d", &num_input);

    // Check if input was not an integer or if it's <= 0
    if (q_result != 1 || num_input < lower) {
      printf("%s", error);

      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      num_input = lower - 1;
    }
  } while (num_input < lower);

  return num_input;
}

int int_input_upper(char *prompt, char *error, int upper) {
  int num_input = upper + 1;
  do {
    printf("%s", prompt);
    int q_result = scanf("%d", &num_input);

    // Check if input was not an integer or if it's <= 0
    if (q_result != 1 || num_input > upper) {
      printf("%s", error);

      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      num_input = upper + 1;
    }
  } while (num_input > upper);

  return num_input;
}

int int_input_lower_upper(char *prompt, char *error, int lower, int upper) {
  int num_input = lower - 1;
  do {
    printf("%s", prompt);
    int q_result = scanf("%d", &num_input);

    // Check if input was not an integer or if it's <= 0
    if (q_result != 1 || num_input < lower || num_input > upper) {
      printf("%s", error);

      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      num_input = lower - 1;
    }
  } while (num_input < lower || num_input > upper);

  return num_input;
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

int find_menu_item(char *item_num, const char *menu[ITEMS][3]) {
  for (int item = 0; item < ITEMS; item++) {
    if (strcmp(item_num, menu[item][0]) == 0) {
      return item;
    }
  }

  return -1;
}

void add_order(int item_index, int quantity, char *orders[ITEMS][4]) {
  char quant[32];

  if (orders[item_index][0] != NULL) {
    int current_quantity =
        atoi(orders[item_index][3]); // Convert existing quantity to int

    int new_quantity = current_quantity + quantity; // Add new quantity
    sprintf(quant, "%d", new_quantity);
    free(orders[item_index][3]); // Free the old quantity string

    orders[item_index][3] = strdup(quant);
    return;
  }

  sprintf(quant, "%d", quantity);
  orders[item_index][0] = strdup(MENU[item_index][0]);
  orders[item_index][1] = strdup(MENU[item_index][1]);
  orders[item_index][2] = strdup(MENU[item_index][2]);
  orders[item_index][3] = strdup(quant);
}

int get_orders_total(char *orders[ITEMS][4]) {
  int total = 0;
  for (int item = 0; item < ITEMS; item++) {
    if (orders[item][2] == NULL)
      continue;
    total += atoi(orders[item][2]) * atoi(orders[item][3]);
  }

  return total;
}

#endif // UTILS_H