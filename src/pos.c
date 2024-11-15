#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "menu.h"
#include "utils.h"

#define SENIOR_RATE 0.2
#define OTHER_RATE 0.3

void print_banner();
int login();
void display_menu();
int get_option();
int print_receipt(char *orders[ITEMS][4]);

int main() {
  // print brand banner
  printf("\n\n");
  print_banner();

  printf("\n\n");
  printf("===========\n");
  printf("   LOGIN\n");
  printf("===========\n");
  // stop program for an invalid login
  if (login() == 0)
    return -1;

  printf("\n\n");
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

  printf("\n****************************************************\n");
  printf("*                    TAKE ORDER                     *\n");
  printf("****************************************************\n");

  // list of orders
  char *orders[ITEMS][4] = {{NULL}};

  int order_count = 1;
  while (1) {
    char order_num[4];
    int quantity = 0;

    // take order via item number
    int item_index = -1;
    while (item_index == -1) {
      if (order_count > 1)
        printf("\n");
      printf("\n####### ITEM %d #######\n", order_count);
      printf("| Enter order: ");
      scanf("%s", order_num);

      item_index = find_menu_item(order_num, MENU);

      if (item_index == -1)
        printf("WARNING: Enter a valid menu item\n");
    }

    printf("| ADDED: %s - %s to the order list\n", MENU[item_index][0],
           MENU[item_index][1]);

    // take order quantity
    quantity = int_input_lower(
        "| Quantity: ", "WARNING: Enter a valid quantity.\n", 1);

    add_order(item_index, quantity, orders);
    printf("| TOTAL: %dPHP\n", get_orders_total(orders));
    order_count++;

    // prompt user whether to add more orders or not
    char add = '0';
    while (add != 'y' && add != 'n') {
      // Clear the input buffer after reading the quantity
      while (getchar() != '\n')
        ; // Consume remaining characters in the buffer

      printf("| Additional Order? (Press y or n): ");
      add = getchar();

      if (add != 'y' && add != 'n')
        printf("WARNING: Enter valid input (y or n).\n");
    }

    // order again
    if (add == 'y')
      continue;

    // proceed to receipt or exit choice
    break;
  }

  printf("\n");
  int receipt_choice = int_input_lower_upper(
      "| Press 1-Receipt, 2-Exit: ", "WARNING: Enter valid choice (1 or 2).\n",
      1, 2);

  if (receipt_choice == 2) {
    printf("Exiting...\n");
    return 1;
  }

  float bill = (float)print_receipt(orders);

  int discount_choice = int_input_lower_upper(
      "| DISCOUNT? (0-None; 1-Senior 20%; 2-Others 30%): ",
      "WARNING: Enter valid choice.\n", 0, 2);

  float senior_discount = 0;
  float other_discount = 0;

  if (discount_choice != 0) {
    senior_discount = (discount_choice == 1) ? SENIOR_RATE * bill : 0;
    other_discount = (discount_choice == 2) ? OTHER_RATE * bill : 0;
  }

  bill -= (senior_discount + other_discount);

  float cash = 0;
  while (cash < bill) {
    printf("| CASH: ");
    int cash_result = scanf("%f", &cash);

    if (cash_result != 1) {
      printf(
          "WARNING: Enter a valid amount. Only numeric input is alllowed.\n");

      // Clear the input buffer if the input was invalid
      while (getchar() != '\n')
        ; // Discard the invalid input

      // Reset `quantity` to an invalid value to keep the loop going
      cash = 0;
      continue;
    }

    if (cash < bill) {
      printf("WARNING: Invalid. Cash should exceed total price.\n");
    }
  }

  printf("| CHANGE: %.2f\n", cash - bill);

  return 0;
}

void print_banner() {
  printf("██████╗ ██╗███████╗███████╗██╗     ███████╗██████╗ ███████╗    "
         "███████╗██╗███████╗███████╗██╗     ███████╗██████╗ ███████╗\n");
  printf("██╔══██╗██║╚══███╔╝╚══███╔╝██║     ██╔════╝██╔══██╗██╔════╝    "
         "██╔════╝██║╚══███╔╝╚══███╔╝██║     ██╔════╝██╔══██╗██╔════╝\n");
  printf("██████╔╝██║  ███╔╝   ███╔╝ ██║     █████╗  ██████╔╝███████╗    "
         "███████╗██║  ███╔╝   ███╔╝ ██║     █████╗  ██████╔╝███████╗\n");
  printf("██╔══██╗██║ ███╔╝   ███╔╝  ██║     ██╔══╝  ██╔══██╗╚════██║    "
         "╚════██║██║ ███╔╝   ███╔╝  ██║     ██╔══╝  ██╔══██╗╚════██║\n");
  printf("██║  ██║██║███████╗███████╗███████╗███████╗██║  ██║███████║    "
         "███████║██║███████╗███████╗███████╗███████╗██║  ██║███████║\n");
  printf("╚═╝  ╚═╝╚═╝╚══════╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝    "
         "╚══════╝╚═╝╚══════╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝\n");
}

int login() {
  char username_buffer[ENCRYPT_SIZE], pw_buffer[ENCRYPT_SIZE];

  const char *username = str_input("USERNAME: ", username_buffer, ENCRYPT_SIZE);
  if (username[0] == '\0') {
    printf("WARNING: Invalid username.\n");
    return 0;
  }

  if (strlen(username) > 20) {
    printf("WARNING: Username should be less than 20 characters.\n");
    return 0;
  }

  const char *password = get_password("PASSWORD: ");
  if (password[0] == '\0') {
    printf("WARNING: Invalid password.\n");
    return 0;
  }

  if (strlen(password) > 20) {
    printf("WARNING: Password should be less than 20 characters.\n");
    return 0;
  }

  // check if username exists in database
  int user_index = find_account(username, DATABASE);

  if (user_index == -1) {
    printf("INFO: No account registered in system.\n");
    return 0;
  }

  if (!verify_pw(user_index, password, DATABASE)) {
    printf("WARNING: Incorrect password.\n");
    return 0;
  }

  return 1;
}

void display_menu() {
  printf("**********************************************\n");
  printf("*          Rizzler's Sizzlers Menu           *\n");
  printf("**********************************************\n");
  printf("| %-5s | %-24s  | %-6s |\n", "Code", "Item", "Price");
  printf("----------------------------------------------\n");

  for (int i = 0; i < ITEMS; i++) {
    printf("| %-5s | %-24s  | %6s |\n", MENU[i][0], MENU[i][1], MENU[i][2]);
  }

  printf("**********************************************\n");
}

int get_option() {
  // display restaurant's options
  printf("\n");
  printf("****************************************************\n");
  printf("*                    OPTIONS                       *\n");
  printf("****************************************************\n");
  printf("| 01 | Back to Menu                                |\n");
  printf("| 02 | Take Order                                  |\n");
  printf("| 03 | Exit                                        |\n");
  printf("****************************************************\n");

  int option = -1;
  do {
    printf("Option: ");
    int res = scanf("%d", &option);
    // Clear the input buffer after reading the option
    while (getchar() != '\n')
      ; // Consume remaining characters in the buffer

    if (res != 1 || option < 1 || option > 3) {
      printf(
          "WARNING: Input valid option (1, 2, or 3). Numeric values only.\n\n");
      option = -1;
    }
  } while (option < 1 || option > 3);

  return option;
}

int print_receipt(char *orders[ITEMS][4]) {
  int total = get_orders_total(orders);

  printf("\n************************************************************\n");
  printf("*                        RECEIPT                           *\n");
  printf("************************************************************\n");
  printf("| Order Code  | Item                        | Qty | Price  |\n");
  printf("------------------------------------------------------------\n");

  for (int item = 0; item < ITEMS; item++) {
    if (orders[item][0] == NULL)
      continue;
    printf("| %-10s  | %-27s | %-3s | %-6s |\n", orders[item][0],
           orders[item][1], orders[item][3], orders[item][2]);
  }

  printf("------------------------------------------------------------\n");
  printf("| TOTAL:                                          %-8d |\n", total);

  return total;
}
