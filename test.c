#include "hw.h"
#include "test.h"

//------------------------------------------------------------------------------
uint8_t isTestEnd()
{
    uint8_t retval = 0;
    
    if (gEvents & EV_UART1_RX) {
	if (gUart2Rx == 'q')
	    retval = 1;
	
	gEvents &= ~(EV_UART1_RX);
    }
    return retval;
}

//------------------------------------------------------------------------------
void longDelay(uint16_t time)
{
    uint16_t i;
    for (i=0; i<time; i++) {
	delay_us(1000);
    }
}  

//------------------------------------------------------------------------------
void testRfm12_1()
{
    uint8_t buffer[] = { 'h', 'e', 'l', 'l', 'o' };

    while (1) {	
	rfm12b_send(&rfm1, buffer, sizeof buffer);
	longDelay(1000);

	if (isTestEnd())
	    break;
    }
}
    
//------------------------------------------------------------------------------
void testRfm12_2()
{
    while (1) {	
	rfm12b_test(&rfm1);
	longDelay(1000);

	if (isTestEnd())
	    break;
    }
}

//------------------------------------------------------------------------------
void testLed()
{
    while (1) {	
	io_set(PIN_LED1);
	io_set(PIN_LED2);
	longDelay(1000);
	
	io_clear(PIN_LED1);
	io_clear(PIN_LED2);
	longDelay(1000);

	if (isTestEnd())
	    break;
    }
}

