#include "hw.h"
#include "hal.h"
#include <termios.h>
#include "buffer.h"
#include "event.h"

//------------------------------------------------------------------------------
void buffer_print(buffer_t *buf)
{
    for (uint8_t i = buf->position;
	 i < buf->size;
	 i++)
    {
	printf("%c", buf->data[i]);
    }
    buffer_clear(buf);

    EVENT_SET(EV_UART1_TX, 0);
}

//------------------------------------------------------------------------------
void uart_send(uint8_t port, char ch)
{
    printf("%c", ch);
}

//------------------------------------------------------------------------------
void uart_sends(uint8_t port, char *buf)
{
    printf("%s", buf);
}

//------------------------------------------------------------------------------
void uart_send_nl(uint8_t port)
{
    printf("\r\n");
}

//------------------------------------------------------------------------------
void event_check()
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

    if (ch != 0xFF) {
	EVENT_SET(EV_UART1_RX, ch);
    }
} 
