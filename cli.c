#include "hw.h"
#include "hal.h"
#include "buffer.h"
#include "meas.h"
#include "scheduler.h"
#include "test.h"

buffer_t outBuffer;

typedef uint8_t (*func_cli)(buffer_t*, buffer_t*);

typedef struct {
    uint16_t cmd;
    func_cli func;
} command_t;

uint8_t cmdUTest(buffer_t *in, buffer_t *out);
uint8_t cmdVersion(buffer_t *in, buffer_t *out);
uint8_t cmdMeasShow(buffer_t *in, buffer_t *out);
uint8_t cmdMeasTrigger(buffer_t *in, buffer_t *out);
uint8_t cmdMeasHighLimit(buffer_t *in, buffer_t *out);
uint8_t cmdMeasLowLimit(buffer_t *in, buffer_t *out);
uint8_t cmdTimerActivate(buffer_t *in, buffer_t *out);
uint8_t cmdTimerShow(buffer_t *in, buffer_t *out);
uint8_t cmdTimerBegin(buffer_t *in, buffer_t *out);
uint8_t cmdTimerEnd(buffer_t *in, buffer_t *out);


#define CMD(a,b) a * 0x100 + b

command_t gCommands[] = {
    {CMD('m', 'h'), cmdMeasHighLimit},
    {CMD('m', 'l'), cmdMeasLowLimit},
    {CMD('m', 's'), cmdMeasShow},
    {CMD('m', 't'), cmdMeasTrigger},
    {CMD('t', 'a'), cmdTimerActivate},
    {CMD('t', 'b'), cmdTimerBegin},
    {CMD('t', 'e'), cmdTimerEnd},
    {CMD('t', 's'), cmdTimerShow},
    {CMD('u', 't'), cmdUTest},
    {CMD('v', 'e'), cmdVersion},
    {0,             NULL}
};

void cli_execute(buffer_t *buf)
{
    uint8_t i, found = 0;
    uint16_t cmd;
    uint8_t err = 0;
    
    buffer_clear(&outBuffer);

    if (BUFFER_SIZE(buf) > 0) {
	cmd = buffer_get(buf) << 8;
	cmd += buffer_get(buf);

	for (i=0; gCommands[i].cmd != 0; i++) {
	    if (cmd == gCommands[i].cmd) {
		err = (*gCommands[i].func)(buf, &outBuffer);
		found = 1;
		break;
	    }
	}
	if (!found) {
	    err = ERR_INVALID_COMMAND;
	}
    }
    
    buffer_nl(&outBuffer);
    if (err) {
	buffer_str( &outBuffer, "ERR:");
	buffer_hex8(&outBuffer, err);
	buffer_nl(  &outBuffer);
    }
    if (gFuncMLine == 0)
	buffer_ch(&outBuffer, '>');
    uart_print(UART, &outBuffer);
    
    buffer_clear(buf);
}

//------------------------------------------------------------------------------
uint8_t char_range(buffer_t *in, char begin, char end, uint8_t *error)
{
    char ch = buffer_get(in);

    if (ch >= begin && ch <= end) {
	return ch - begin;
    }
    *error = 1;
    return 0;
}

void char_equal(buffer_t *in, char comp, uint8_t *error)
{
    char ch = buffer_get(in);

    if (ch != comp) {
	*error = 1;
    }
}

//------------------------------------------------------------------------------
uint8_t cmdUTest(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    
    char_equal(in, ' ', &err);
    
    uint16_t arg = \
	char_range(in, '0', '9', &err) * 100 +	\
	char_range(in, '0', '9', &err) * 10 + \
	char_range(in, '0', '9', &err);

    if (err) {
	return ERR_ARGUMENT;
    }

    switch (arg) {
    case 1:
	testPrn1();
	break;
    case 2:
	testPrn2();
	break;
    case 3:
	testLed();
	break;
    }    
    return 0;
}

void test2(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    
    if (buffer_get(in) != ' ') {
	_error(ERR_SYNTAX);
	return;
    }

    uint16_t hour =			      \
	char_range(in, '0', '2', &err) * 10 + \
	char_range(in, '0', '9', &err);

    uint16_t min =			      \
	char_range(in, '0', '5', &err) * 10 + \
	char_range(in, '0', '9', &err);

    if (err) {
	_error(ERR_ARGUMENT);
	return;
    }
    
    buffer_str(out, "\nTEST2 ");
    buffer_dec(out, hour);
    buffer_ch(out, ':');
    buffer_dec(out, min);
}

uint8_t cmdVersion(buffer_t *in, buffer_t *out)
{
    buffer_nl(out);
    buffer_str(out, gVersion);

    return 0;
}

uint8_t cmdMeasShow(buffer_t *in, buffer_t *out)
{
    buffer_nl(out);
    meas_show(1, out);
    gFuncMLine = meas_show;

    return 0;
}

uint8_t cmdMeasTrigger(buffer_t *in, buffer_t *out)
{
    return 0;
}

uint8_t cmdMeasHighLimit(buffer_t *in, buffer_t *out)
{
    return 0;
}

uint8_t cmdMeasLowLimit(buffer_t *in, buffer_t *out)
{
    return 0;
}

uint8_t cmdTimerActivate(buffer_t *in, buffer_t *out)
{
    return 0;
}

uint8_t cmdTimerShow(buffer_t *in, buffer_t *out)
{
    buffer_nl(out);
    sche_show(1, out);
    gFuncMLine = sche_show;

    return 0;
}

static void arg_timer(buffer_t *in,
		      uint8_t *timer,
		      uint8_t *hour,
		      uint8_t *min,
		      uint8_t *err)
{
    char_equal(in, ' ', err);

    *timer = char_range(in, 'a', 'f', err);

    char_equal(in, ' ', err);
    
    *hour =			      \
	char_range(in, '0', '2', err) * 10 + \
	char_range(in, '0', '9', err);

    char_equal(in, ':', err);

    *min =			      \
	char_range(in, '0', '5', err) * 10 + \
	char_range(in, '0', '9', err);    
}

uint8_t cmdTimerBegin(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    uint8_t timer = 0;
    uint8_t hour = 0;
    uint8_t min = 0;

    arg_timer(in, &timer, &hour, &min, &err);

    if (err == 0)
	sche_set_start(timer, hour, min);
    else
	err = ERR_ARGUMENT;

    return err;
}

uint8_t cmdTimerEnd(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    uint8_t timer = 0;
    uint8_t hour = 0;
    uint8_t min = 0;

    arg_timer(in, &timer, &hour, &min, &err);

    if (err == 0)
	sche_set_stop(timer, hour, min);
    else
	err = ERR_ARGUMENT;

    return err;
}
