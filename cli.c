#include "hw.h"
#include "hal.h"
#include "buffer.h"
#include "meas.h"
#include "scheduler.h"

buffer_t outBuffer;

typedef void (*func_cli)(buffer_t*, buffer_t*);

typedef struct {
    uint16_t cmd;
    func_cli func;
} command_t;

void cmdUTest(buffer_t *in, buffer_t *out);
void cmdVersion(buffer_t *in, buffer_t *out);
void cmdMeasShow(buffer_t *in, buffer_t *out);
void cmdMeasTrigger(buffer_t *in, buffer_t *out);
void cmdMeasHighLimit(buffer_t *in, buffer_t *out);
void cmdMeasLowLimit(buffer_t *in, buffer_t *out);
void cmdTimerActivate(buffer_t *in, buffer_t *out);
void cmdTimerShow(buffer_t *in, buffer_t *out);
void cmdTimerBegin(buffer_t *in, buffer_t *out);
void cmdTimerEnd(buffer_t *in, buffer_t *out);


#define CMD(a,b) a * 0x100 + b

command_t gCommands[] = {
    {CMD('m', 's'), cmdMeasShow},
    {CMD('m', 't'), cmdMeasTrigger},
    {CMD('m', 'h'), cmdMeasHighLimit},
    {CMD('m', 'l'), cmdMeasLowLimit},
    {CMD('t', 'a'), cmdTimerActivate},
    {CMD('t', 's'), cmdTimerShow},
    {CMD('t', 'b'), cmdTimerBegin},
    {CMD('t', 'e'), cmdTimerEnd},
    {CMD('u', 't'), cmdUTest},
    {CMD('v', 'e'), cmdVersion},
    {0,             NULL}
};

void cli_execute(buffer_t *buf)
{
    uint8_t i, found = 0;
    uint16_t cmd;
    buffer_clear(&outBuffer);

    cmd = buffer_get(buf) << 8;
    cmd += buffer_get(buf);

    for (i=0; gCommands[i].cmd != 0; i++) {
	if (cmd == gCommands[i].cmd) {
	    (*gCommands[i].func)(buf, &outBuffer);
	    buffer_nl(&outBuffer);
	    uart_print(UART, &outBuffer);
	    found = 1;
	    break;
	}
    }
    if (!found) {
	error(ERR_INVALID_COMMAND);
	uart_print(UART, buf);
    }
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
void cmdUTest(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    
    char_equal(in, ' ', &err);
    
    uint16_t arg = \
	char_range(in, '0', '9', &err) * 100 +	\
	char_range(in, '0', '9', &err) * 10 + \
	char_range(in, '0', '9', &err);

    if (err) {
	error(ERR_ARGUMENT);
	return;
    }
    
    buffer_str(out, "\nTEST1 ");
    buffer_hex16(out, arg);
}

void test2(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    
    if (buffer_get(in) != ' ') {
	error(ERR_SYNTAX);
	return;
    }

    uint16_t hour =			      \
	char_range(in, '0', '2', &err) * 10 + \
	char_range(in, '0', '9', &err);

    uint16_t min =			      \
	char_range(in, '0', '5', &err) * 10 + \
	char_range(in, '0', '9', &err);

    if (err) {
	error(ERR_ARGUMENT);
	return;
    }
    
    buffer_str(out, "\nTEST2 ");
    buffer_dec(out, hour);
    buffer_ch(out, ':');
    buffer_dec(out, min);
}

void cmdVersion(buffer_t *in, buffer_t *out)
{
    buffer_str(out, "\nV0.0.1");
}

void cmdMeasShow(buffer_t *in, buffer_t *out)
{
    buffer_nl(out);
    meas_show(1, out);
    gFuncMLine = meas_show;
}

void cmdMeasTrigger(buffer_t *in, buffer_t *out)
{
}

void cmdMeasHighLimit(buffer_t *in, buffer_t *out)
{
}

void cmdMeasLowLimit(buffer_t *in, buffer_t *out)
{
}

void cmdTimerActivate(buffer_t *in, buffer_t *out)
{
}

void cmdTimerShow(buffer_t *in, buffer_t *out)
{
    buffer_nl(out);
    sche_show(1, out);
    gFuncMLine = sche_show;
}

void cmdTimerBegin(buffer_t *in, buffer_t *out)
{
    uint8_t err = 0;
    uint8_t timer;
    
    char_equal(in, ' ', &err);

    timer = char_range(in, 'a', 'f', &err);

    char_equal(in, ' ', &err);
    
    uint16_t arg = \
	char_range(in, '0', '9', &err) * 100 +	\
	char_range(in, '0', '9', &err) * 10 + \
	char_range(in, '0', '9', &err);
}

void cmdTimerEnd(buffer_t *in, buffer_t *out)
{
}
