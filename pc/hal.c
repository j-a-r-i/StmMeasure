#include "hw.h"
#include "hal.h"
#include <termios.h>
#include <unistd.h>
#include "buffer.h"
#include "event.h"

extern void USART2_IRQHandler();

//------------------------------------------------------------------------------
void delay_us(uint16_t ms)
{
    printf("[delay]\n");
}

void io_set(pin_t pin)
{
    printf("[set %d]\n", pin);
}

void io_clear(pin_t pin)
{
    printf("[clr %d]\n", pin);
}

//------------------------------------------------------------------------------
uint8_t read_char()
{
    struct termios tio;
    uint8_t ch, tmp1, tmp2;

    tcgetattr(0, &tio);
    tio.c_lflag &= ~ICANON;
    tio.c_lflag &= ~ECHO;

    tmp1 = tio.c_cc[VMIN];
    tmp2 = tio.c_cc[VTIME];
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;
    
    tcsetattr(0, TCSANOW, &tio);

    ch = getchar();

    tio.c_lflag |= ICANON;
    tio.c_lflag |= ECHO;
    tio.c_cc[VMIN] = tmp1;
    tio.c_cc[VTIME] = tmp2;

    if (ch == 0xFF)
	ch = 0;
    return ch;
} 

//------------------------------------------------------------------------------
void *uart_thread(void *ptr)
{
    USART_TypeDef *port = (USART_TypeDef*)ptr;
    uint8_t ch;

    port->TXE = 1;
    port->TDR = 0;
    port->RXNE = 0;
    port->RDR = 0;

    printf("uart thread...\n");
    while (1) {
	if (port->TDR != 0) {
	    port->TXE = 0;
	    printf("%c", port->TDR);
	    usleep(3000);  // 3 ms sleep simulated uart sending delay
	    port->TXE = 1;
	    port->TDR = 0;

	    if (port->INT_TXE)
		USART2_IRQHandler();
	}

	ch = read_char();
	if (ch) {
	    port->RDR = ch;
	    port->RXNE = 1;
	    USART2_IRQHandler();
	    port->RXNE = 0;
	    port->RDR  = 0;
	}   
    }
}
