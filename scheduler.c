#include "hw.h"
#include "scheduler.h"

sched_t gScheduleTable[MAX_SCHEDULE];


void show_time(time_t *time, buffer_t *buf)
{
    if (time->enabled) {
	buffer_dec( buf, time->hour);
	buffer_ch(  buf, ':');
	buffer_dec( buf, time->min);
    }
    else {
	buffer_str( buf, "__:__");
    }
}

//------------------------------------------------------------------------------
/** print out schedule table entry
 *  timer value can be 0.. (MAX_SCHEDULE - 1)
 *
 *  @param first  is first entry to print
 *  @param buf    entry info is filled into this
 *  @return       is this last entry
 */
uint8_t sche_show(uint8_t first, buffer_t *buf)
{
    static uint8_t timer = 0;

    if (first)
	timer = 0;
    else
	timer++;

    if (timer >= MAX_SCHEDULE) {
	buffer_str(buf, "overflow");
	return 1;
    }
    
    buffer_ch(  buf, 'a' + timer);
    buffer_ch(  buf, ' ');
    show_time(&gScheduleTable[timer].start, buf);
    buffer_ch(  buf, ' ');
    show_time(&gScheduleTable[timer].stop, buf);
    buffer_ch(  buf, ' ');
        
    return timer == (MAX_SCHEDULE - 1);
}

void sche_set_start(uint8_t timer, uint8_t hour, uint8_t min)
{
    if (timer < MAX_SCHEDULE) {
	gScheduleTable[timer].start.hour = hour;
	gScheduleTable[timer].start.min  = min;
	gScheduleTable[timer].start.enabled = 1;
    }
    else
	_error(ERR_INVALID_SCHE);
}

void sche_set_stop(uint8_t timer, uint8_t hour, uint8_t min)
{
    if (timer < MAX_SCHEDULE) {
	gScheduleTable[timer].stop.hour = hour;
	gScheduleTable[timer].stop.min  = min;
	gScheduleTable[timer].stop.enabled = 1;
    }
    else
	_error(ERR_INVALID_SCHE);
}
