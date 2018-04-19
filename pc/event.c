#include "hw.h"
#include "event.h"

uint32_t gEvents2;


void event_init()
{
    gEvents2 = 0;
}

void event_handle()
{
    if (gEvents2 == 0)
	return;
    
    for(uint8_t bit=0; bit<EV_MAX; bit++) {
	if (gEvents2 & (1<<bit)) {
	    gEvents2 &= ~(1<<bit);

	    (*(gEvents2Table[bit].func))(gEvents2Table[bit].arg);
	}
    }
}
