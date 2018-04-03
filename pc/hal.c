#include "hw.h"
#include "hal.h"
#include <termios.h>

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
uint8_t event_loop()
{
    struct termios tio;
    uint8_t ch;

    tcgetattr(0, &tio);
    tio.c_lflag &= ~ICANON;
    tio.c_lflag &= ~ECHO;

    tcsetattr(0, TCSANOW, &tio);

    ch = getchar();

    tio.c_lflag |= ICANON;
    tio.c_lflag |= ECHO;

    return ch;
} 
