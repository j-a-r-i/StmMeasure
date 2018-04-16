#include "hw.h"
#include "event.h"


uint32_t gEvents2;

#define EVENT_SET(ev,arg) gEvents |= (1<<ev); gEventsTable[ev].arg = arg 

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

	    *(gEvents2Table[i].func)(gEvents2Table[i].arg);
	}
    }
}
