#include "hw.h"
#include "meas.h"
#include "buffer.h"

meas_t gMeasureTable[MAX_MEASURE];

//------------------------------------------------------------------------------
/** Print out measure table entry.
 *  meas variable can be 0 .. (MAX_MEASURE - 1)
 *  @param first  is first entry to print
 *  @param buf    buffer where entry info is filled
 *  @return       is this last entry
 */
uint8_t meas_show(uint8_t first, buffer_t *buf)
{
    static uint8_t meas = 0;

    if (first)
	meas = 0;
    else
	meas++;

    if (meas >= MAX_MEASURE) {
	buffer_str(buf, "overflow");
	return 1;
    }
    
    buffer_ch(   buf, 'a' + meas);
    buffer_ch(   buf, ' ');
    buffer_hex16(buf, gMeasureTable[meas].value);
    buffer_ch(   buf, ' ');
    buffer_hex16(buf, gMeasureTable[meas].highLimit);
    buffer_ch(   buf, ' ');
    buffer_hex16(buf, gMeasureTable[meas].lowLimit);

    return meas == (MAX_MEASURE - 1);
}
		
void meas_set_high_limit(uint8_t meas, uint16_t val)
{
    if (meas < MAX_MEASURE)
	gMeasureTable[meas].highLimit = val;
    else
	_error(ERR_INVALID_MEAS);
}

void meas_set_low_limit(uint8_t meas, uint16_t val)
{
    if (meas < MAX_MEASURE)
	gMeasureTable[meas].lowLimit = val;
    else
	_error(ERR_INVALID_MEAS);
}
