#include "hw.h"
#include "test.h"
#include "buffer.h"
#include "event.h"

buffer_t tstBuffer;
static uint8_t    gRunning;
static func_event gOrgEvUart;

//------------------------------------------------------------------------------
void longDelay(uint16_t time)
{
#ifdef SIMULATED
    delay_us(1000);
#else
    uint16_t i;
    for (i=0; i<time; i++) {
	delay_us(1000);
    }
#endif
}  

//------------------------------------------------------------------------------
void on_tst_uart_rx(uint8_t ch)
{
    if (ch == 'q') {
	gRunning = 0;
    }
}

/*
//------------------------------------------------------------------------------
void testRfm12_1()
{
    uint8_t buffer[] = { 'h', 'e', 'l', 'l', 'o' };

    gRunning = 1;
    
    while (gRunning) {	
	rfm12b_send(&rfm1, buffer, sizeof buffer);
	longDelay(1000);

	event_handle();
    }
}
    
//------------------------------------------------------------------------------
void testRfm12_2()
{
    gRunning = 1;
    
    while (gRunning) {	
	rfm12b_test(&rfm1);
	longDelay(1000);

	event_handle();
    }
}
*/

//------------------------------------------------------------------------------
void testLed()
{
    gRunning = 1;
    gOrgEvUart = gEvents2Table[EV_UART1_RX].func;
    gEvents2Table[EV_UART1_RX].func = on_tst_uart_rx;
    
    while (gRunning) {	
	io_set(PIN_LED1);
	io_set(PIN_LED2);
	longDelay(1000);
	
	io_clear(PIN_LED1);
	io_clear(PIN_LED2);
	longDelay(1000);

	event_handle();
    }

    gEvents2Table[EV_UART1_RX].func = gOrgEvUart;
}


//------------------------------------------------------------------------------
void testPrn1()
{
    static uint8_t counter = 0;
    
    buffer_clear(&tstBuffer);
    buffer_str(  &tstBuffer, "test ");
    buffer_hex8( &tstBuffer, counter);
    buffer_nl(   &tstBuffer);

    uart_print(UART, &tstBuffer);
    
    counter++;
}


//------------------------------------------------------------------------------
void testPrn2()
{
    static uint8_t counter = 0;
    
    buffer_clear(&tstBuffer);
    buffer_str(  &tstBuffer, "test ");
    buffer_hex8( &tstBuffer, counter);
    buffer_nl(   &tstBuffer);
    uart_print(UART, &tstBuffer);

    buffer_clear(&tstBuffer);
    buffer_str(  &tstBuffer, "second ");
    buffer_hex8( &tstBuffer, counter);
    buffer_nl(   &tstBuffer);
    uart_print(UART, &tstBuffer);

    counter++;
}
