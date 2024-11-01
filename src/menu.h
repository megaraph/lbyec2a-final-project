// menu.h
#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h>

#include <stddef.h>

#include "config.h"

static const char *MENU[ITEMS][3] = {
    {"011", "Tiny Burger", "20"},
    {"022", "Jumbo Burger", "80"},
    {"033", "Turkey Burger", "85"},
    {"044", "Chicken Burger", "90"},
    {"055", "BBQ Burger", "90"},
    {"066", "Banquet Burger", "150"},
    {"077", "Chili Burger", "100"},
    {"088", "California Burger", "120"},
    {"099", "Jucy Lucy", "130"},
    {"100", "Rice Burger", "120"},
    {"111", "Salmon Burger", "150"},
    {"122", "Slopper", "120"},
    {"133", "Slug Burger", "110"},
    {"144", "Veggie Burger", "150"},
    {"155", "Teriyaki Burger", "160"},
    {"166", "Big Fries", "60"},
    {"177", "Mojos", "60"},
    {"188", "Large Drinks", "30"},
    {"199", "Water", "20"},
    {"200", "Extra Cheese", "15"},
};

#endif // MENU_H