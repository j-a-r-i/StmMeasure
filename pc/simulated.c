#include "hw.h"
#include "buffer.h"
#include "cli.h"
#include "event.h"

func_mline gFuncMLine;

extern void event_check();
extern void buffer_print(buffer_t *);

buffer_t buf;
buffer_t inBuffer;

#define KEY_ENTER 10
#define KEY_BACKSP 127

//------------------------------------------------------------------------------
void error(error_t code)
{
    if (code == ERR_BUFFER_OVERFLOW) {
	uart_print(UART, &buf);
    }
    buffer_clear(&buf);
    buffer_str(&buf, "\nERROR:");
    buffer_hex8(&buf, code);
    buffer_nl(&buf);

    uart_print(UART, &buf);
}

//------------------------------------------------------------------------------
void trace(uint8_t ch)
{
    buffer_ch(&buf, '[');
    buffer_ch(&buf, ch);
    buffer_ch(&buf, ']');
}

//------------------------------------------------------------------------------
void show_version()
{
    buffer_clear(&buf);
    buffer_str(&buf, "\nsimulated version\n");

    uart_print(UART, &buf);
}

void dump(uint8_t val)
{
    buffer_clear(&buf);
    buffer_hex8(&buf, val);
    uart_print(UART, &buf);    
}

void show_prompt()
{
    uart_send(UART, '>');
}


//------------------------------------------------------------------------------
void on_uart_rx(uint8_t ch)
{
    switch (ch) {
    case KEY_ENTER:
	cli_execute(&inBuffer);
	if (gFuncMLine == 0)
	    show_prompt();
	break;
    case KEY_BACKSP:
	buffer_remove(&inBuffer);
	uart_send(UART, ch);
	break;
    default:
	buffer_ch(&inBuffer, ch);
	uart_send(UART, ch);
    }
}

void on_uart_tx(uint8_t ch)
{
    uint8_t last;

    if (gFuncMLine != NULL) {
	buffer_clear(&buf);
	last = (*gFuncMLine)(0, &buf);
	buffer_nl(&buf);
	uart_print(UART, &buf);
	if (last) {
	    show_prompt();
	    gFuncMLine = NULL;
	}
    }
}

void on_null(uint8_t ch)
{
}

event_t gEvents2Table[] = {
    [EV_TIMER2]   = { 0, on_null },
    [EV_UART1_RX] = { 0, on_uart_rx },
    [EV_UART1_TX] = { 0, on_uart_tx },
    [EV_UART2_RX] = { 0, on_null },
    [EV_UART2_TX] = { 0, on_null },
    [EV_USER1]    = { 0, on_null },
    [EV_USER2]    = { 0, on_null },
};

 

//------------------------------------------------------------------------------
void main(int argc, char **argv)
{
    gFuncMLine = NULL;

    event_init();
    buffer_clear(&inBuffer);
    show_prompt();
    
    while (1) {
	event_check();

	event_handle();
    }
}
