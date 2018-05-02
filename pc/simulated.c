#include "hw.h"
#include "buffer.h"
#include "cli.h"
#include "event.h"
#include <pthread.h>
#include <unistd.h>


extern void buffer_print(buffer_t *);
extern void *uart_thread(void *ptr);

USART_TypeDef gUart1;
USART_TypeDef gUart2;
USART_TypeDef *USART1;
USART_TypeDef *USART2;

func_mline gFuncMLine;
buffer_t buf;
buffer_t inBuffer;
const char *gVersion = "SIM 0.1";

#define PROMPT '>'

#define KEY_ENTER 10
#define KEY_BACKSP 127

//------------------------------------------------------------------------------
void _error(error_t code)
{
    buffer_clear(&buf);
    buffer_nl(&buf);
    buffer_str(&buf, "ERR:");
    buffer_hex8(&buf, code);
    buffer_nl(&buf);
    buffer_ch(&buf, PROMPT);

    uart_sync(UART, &buf);
}

//------------------------------------------------------------------------------
void trace(uint8_t ch)
{
    buffer_ch(&buf, '[');
    buffer_ch(&buf, ch);
    buffer_ch(&buf, ']');
}

//------------------------------------------------------------------------------
void dump(uint8_t val)
{
    buffer_clear(&buf);
    buffer_hex8(&buf, val);
    uart_print(UART, &buf);    
}

void show_prompt()
{
    uart_send(UART, PROMPT);
}


//------------------------------------------------------------------------------
void on_uart_rx(uint8_t ch)
{
    switch (ch) {
    case KEY_ENTER:
	cli_execute(&inBuffer);
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
	if (last) {
	    buffer_ch(&buf, PROMPT);
	    gFuncMLine = NULL;
	}
	uart_print(UART, &buf);
    }
}

void on_null(uint8_t ch)
{
}

event_t gEvents2Table[] = {
    [EV_TIMER2]   = { 0, on_null },
    [EV_UART1_RX] = { 0, on_null },
    [EV_UART1_TX] = { 0, on_null },
    [EV_UART2_RX] = { 0, on_uart_rx },
    [EV_UART2_TX] = { 0, on_uart_tx },
    [EV_USER1]    = { 0, on_null },
    [EV_USER2]    = { 0, on_null },
};


//------------------------------------------------------------------------------
void main(int argc, char **argv)
{
    pthread_t threadUart;
    int ret;
    
    gFuncMLine = NULL;

    USART1 = &gUart1;
    USART2 = &gUart2;

    ret = pthread_create(&threadUart, NULL, uart_thread, USART2);
    usleep(1000);
    
    event_init();
    buffer_clear(&inBuffer);

    printf("mainloop..\n");
    show_prompt();
    while (1) {
	event_handle();
    }
}
