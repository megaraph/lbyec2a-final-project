#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "menu.h"
#include "utils.h"

#define SENIOR_RATE 0.2
#define OTHER_RATE 0.3

int login();
void display_menu();
int get_option();
int print_receipt(char *orders[ITEMS][4]);

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

    // go back to menu
    if (option == 1) {
      continue;
    }

    // continue to ordering
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

    // take order via item number
    int item_index = -1;
    while (item_index == -1) {
      printf("Enter order: ");
      scanf("%s", order_num);

      item_index = find_menu_item(order_num, MENU);

      if (item_index == -1)
        printf("Enter a valid menu item\n");
    }

    // take order quantity
    quantity = int_input_lower("Quantity: ", "Enter a valid quantity.\n", 1);

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

  int receipt_choice = 0;
  while (receipt_choice < 1 || receipt_choice > 2) {
    printf("Press 1-Receipt, 2-Exit: ");
    int r_result = scanf("%d", &receipt_choice);

    if (r_result != 1 || receipt_choice < 1 || receipt_choice > 2) {
      printf("Enter valid choice.\n");
      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      receipt_choice = 0;
    }
  }

  if (receipt_choice == 2) {
    printf("Exiting...\n");
    return 1;
  }

  float bill = (float)print_receipt(orders);

  int discount_choice = -1;
  while (discount_choice < 0 || discount_choice > 2) {
    printf("Discount? (0-None; 1-Senior 20percent, 2-Others 30percent): ");
    int d_result = scanf("%d", &discount_choice);
    if (d_result != 1 || discount_choice < 0 || discount_choice > 2) {
      printf("Enter valid choice.\n");
      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      discount_choice = 0;
    }

    float senior_discount = 0;
    float other_discount = 0;

    if (discount_choice != 0) {
      senior_discount = (discount_choice == 1) ? SENIOR_RATE * bill : 0;
      other_discount = (discount_choice == 2) ? OTHER_RATE * bill : 0;
    }

    bill -= (senior_discount + other_discount);

    float cash = 0;
    while (cash < bill) {
      printf("CASH: ");
      scanf("%f", &cash);

      if (cash < bill) {
        printf("Invalid. Cash should exceed total price.\n");
      }
    }

    printf("CHANGE: %.2f\n", cash - bill);

    return 0;
  }
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

int print_receipt(char *orders[ITEMS][4]) {
  int total = get_orders_total(orders);
  printf("\nRECEIPT\n");
  printf("Order\tQuantity\tPrice\n");

  for (int item = 0; item < ITEMS; item++) {
    if (orders[item][0] == NULL)
      continue;
    printf("%s %s\t%s\t%s\n", orders[item][0], orders[item][1], orders[item][3],
           orders[item][2]);
  }

  printf("TOTAL\t\t%d\n", total);

  return total;
}
