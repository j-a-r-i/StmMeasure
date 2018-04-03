#pragma once

typedef struct MenuItem {
    char key;
    char *description;
    void (*func)();
} menuitem_t;

extern void menu_print(menuitem_t menu[]);
extern void menu_select(menuitem_t menu[], uint8_t key);
