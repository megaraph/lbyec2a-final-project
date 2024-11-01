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