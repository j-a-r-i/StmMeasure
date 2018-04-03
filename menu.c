#include "hw.h"
#include "hal.h"
#include "menu.h"

void menu_print(menuitem_t menu[])
{
    uint8_t i;

    uart_sends(UART, "\r\nMenu:\r\n\n");
    for (i=0; menu[i].key != 0; i++) {
	uart_send(UART, ' ');	
	uart_send(UART, menu[i].key);
	uart_send(UART, ' ');	
	uart_sends(UART, menu[i].description);
	uart_nl(UART);
    }
}

void menu_select(menuitem_t menu[], uint8_t key)
{
    uint8_t i;
    uint8_t found = 0;

    for (i=0; menu[i].key != 0; i++) {
	if (menu[i].key == key) {
	    (*(menu[i].func))();
	    found = 1;
	    break;
	}
    }

    if (!found) {
	uart_sends(UART, "invalid commmand!\r\n");
    }
}
