#include "hw.h"
#include "meas.h"

meas_t gMeasureTable[MAX_MEASURE];

void meas_show()
{
	uint8_t i;

	for (i=0; i < MAX_MEASURE; i++) {
		uart_hex8( UART, i);
		uart_send( UART, ' ');
		uart_hex16(UART, gMeasureTable[i].value);
		uart_send( UART, ' ');
		uart_hex16(UART, gMeasureTable[i].highLimit);
		uart_send( UART, ' ');
		uart_hex16(UART, gMeasureTable[i].lowLimit);
		uart_nl(UART);
	}
}
		
