#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "menu.h"
#include "utils.h"

int login();
void display_menu();
int get_option();

int main() {
  // stop program for an invalid login
  if (login() == 0)
    return -1;

  while (1) {
    // display the menu when successfully logged in
    display_menu();

    // stop at menu while enter key is not pressed
    printf("\n");
    printf("Press enter to continue...");
    while (getchar() != '\n')
      ;

    int option = get_option();

    if (option == 1) {
      continue;
    }

    if (option == 2) {
      break;
    }

    printf("Exiting...\n");
    return 1;
  }

  printf("\n");
  printf("Take Order\n");
  printf("\n");

  // list of orders
  char *orders[ITEMS][4] = {{NULL}};

  while (1) {
    char order_num[4];
    int quantity = 0;

    // take order number
    int item_index = -1;
    while (item_index == -1) {
      printf("Enter order: ");
      scanf("%s", order_num);
      item_index = find_menu_item(order_num, MENU);
      if (item_index == -1)
        printf("Enter a valid menu item\n");
    }

    // take order quantity
    do {
      printf("Quantity: ");
      scanf("%d", &quantity);
      if (quantity <= 0)
        printf("Enter valid quantity.\n");
    } while (quantity <= 0);

    add_order(item_index, quantity, orders);
    printf("TOTAL: %dPHP\n", get_orders_total(orders));

    // prompt user whether to add more orders or not
    char add = '0';
    while (add != 'y' && add != 'n') {
      // Clear the input buffer after reading the quantity
      while (getchar() != '\n')
        ; // Consume remaining characters in the buffer

      printf("Additional Order? (Press y or n): ");
      add = getchar();

      if (add != 'y' && add != 'n')
        printf("Enter valid input (y or n).\n");
    }

    if (add == 'y')
      continue;

    break;
  }

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

int get_option() {
  // display restaurant's options
  printf("\nOPTIONS\n");
  printf("\n");

  printf("01 Back to Menu\n");
  printf("02 Take Order\n");
  printf("03 Exit \n");
  printf("\n");

  int option;
  do {
    printf("Option: ");
    scanf("%d", &option);
    // Clear the input buffer after reading the option
    while (getchar() != '\n')
      ; // Consume remaining characters in the buffer
  } while (option < 1 || option > 3);

  return option;
}
