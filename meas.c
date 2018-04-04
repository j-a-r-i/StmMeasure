#include "hw.h"
#include "meas.h"

meas_t gMeasureTable[MAX_MEASURE];

void meas_show()
{
    uint8_t i;

    uart_nl(UART);
    for (i=0; i < MAX_MEASURE; i++) {
	uart_send( UART, 'a' + i);
	uart_send( UART, ' ');
	uart_hex16(UART, gMeasureTable[i].value);
	uart_send( UART, ' ');
	uart_hex16(UART, gMeasureTable[i].highLimit);
	uart_send( UART, ' ');
	uart_hex16(UART, gMeasureTable[i].lowLimit);
	uart_nl(UART);
    }
}
		
void meas_set_high_limit(uint8_t meas, uint16_t val)
{
    if (meas < MAX_MEASURE)
	gMeasureTable[meas].highLimit = val;
    else
	error(ERR_INVALID_MEAS);
}

void meas_set_low_limit(uint8_t meas, uint16_t val)
{
    if (meas < MAX_MEASURE)
	gMeasureTable[meas].lowLimit = val;
    else
	error(ERR_INVALID_MEAS);
}
