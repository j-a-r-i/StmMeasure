#include "hw.h"
#include "scheduler.h"

sched_t gScheduleTable[MAX_SCHEDULE];

//------------------------------------------------------------------------------
void sche_show()
{
    uint8_t i;

    uart_nl(UART);
    for (i=0; i < MAX_SCHEDULE; i++) {
	uart_send( UART, 'a' + i);
	uart_send( UART, ' ');
	uart_dec8( UART, gScheduleTable[i].start.hour);
	uart_send( UART, ':');
	uart_dec8( UART, gScheduleTable[i].start.min);
	uart_send( UART, ' ');
	uart_dec8( UART, gScheduleTable[i].stop.hour);
	uart_send( UART, ':');
	uart_dec8( UART, gScheduleTable[i].stop.min);
	uart_nl(UART);
    }
}

void sche_set_start(uint8_t timer, uint8_t hour, uint8_t min)
{
    if (timer < MAX_SCHEDULE) {
	gScheduleTable[timer].start.hour = hour;
	gScheduleTable[timer].start.min  = min;	
    }
    else
	error(ERR_INVALID_SCHE);
}

void sche_set_stop(uint8_t timer, uint8_t hour, uint8_t min)
{
    if (timer < MAX_SCHEDULE) {
	gScheduleTable[timer].stop.hour = hour;
	gScheduleTable[timer].stop.min  = min;	
    }
    else
	error(ERR_INVALID_SCHE);
}
