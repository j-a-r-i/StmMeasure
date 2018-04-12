#include "hw.h"
#include "menu2.h"
#include "buffer.h"
#include "meas.h"
#include "scheduler.h"

#define NULL (void*)0

buffer_t gMnuBuffer;

void act_timer_activate(uint8_t len, uint8_t *args);
void act_timer_begin(uint8_t len, uint8_t *args);
void act_timer_end(uint8_t len, uint8_t *args);
void act_meas_high(uint8_t len, uint8_t *args);
void act_meas_low(uint8_t len, uint8_t *args);
void act_set_clock(uint8_t len, uint8_t *args);
 
void menu_meas(uint8_t cmd, context_t *context);
void menu_timer(uint8_t cmd, context_t *context);
void menu_settings(uint8_t cmd, context_t *context);
void menu_utest(uint8_t cmd, context_t *context);
void arg_timer(uint8_t cmd, context_t *context);
void arg_meas(uint8_t cmd, context_t *context);
void arg_on_off(uint8_t cmd, context_t *context);
void arg_clock(uint8_t cmd, context_t *context);
void arg_value(uint8_t cmd, context_t *context);

#define EVENT_BACKSP 127
#define EVENT_ENTER   10

#define TIMER_BEGIN 'a'
#define TIMER_END   'a' + MAX_SCHEDULE
#define MEAS_BEGIN 'a'
#define MEAS_END   'a' + MAX_MEASURE

//------------------------------------------------------------------------------
void invalid_character(context_t *context)
{
    //error(ERR_INVALID_MEAS);
    //(*(context->state))(EVENT_PROMPT, context);
}

//------------------------------------------------------------------------------
void reset_action(context_t *context)
{
    if (context != NULL) {
	context->action = NULL;
	context->currentArg = 0;
	context->substate = 0;
    }
}

void change_state(context_t *context, state_func state)
{
    context->state = state;

    (*state)(EVENT_HELP, context);
}

void add_arg(context_t *context, uint8_t value)
{
    if (context->currentArg < MAX_ARG) {
	context->args[context->currentArg] = value;
	(context->currentArg)++;
    }
    else {
	error(ERR_ARG_COUNT);
    }	
}

void execute(context_t *context)
{
    if (context->action != NULL) {
	(*(context->action))(context->currentArg, context->args);
    }
}

//------------------------------------------------------------------------------

void dump_args(uint8_t len, uint8_t *args)
{
    uint8_t i;

    buffer_str(&gMnuBuffer, "\nARG=");
    for (i=0; i<len; i++) {
	buffer_ch(&gMnuBuffer, ' ');
	buffer_hex8(&gMnuBuffer, args[i]);
    }
    buffer_nl(&gMnuBuffer);
}

void act_timer_activate(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
}

void act_timer_begin(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
    sche_set_start(args[0],
		   args[1]*10 + args[2],
		   args[3]*10 + args[4]);
}

void act_timer_end(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
    sche_set_stop(args[0],
		  args[1]*10 + args[2],
		  args[3]*10 + args[4]);
}

void act_set_clock(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
}

void act_meas_high(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
    meas_set_high_limit(args[0], args[1]*100 + args[2]*10 + args[3]);
}

void act_meas_low(uint8_t len, uint8_t *args)
{
    dump_args(len, args);
    meas_set_low_limit(args[0], args[1]*100 + args[2]*10 + args[3]);
}

//------------------------------------------------------------------------------
void menu_root(uint8_t event, context_t *context)
{
#define ROOT_PROMPT "\nR>"

    switch (event) {
    case EVENT_HELP:
	buffer_str(&gMnuBuffer, "\n\nMeas Timer Settings Version Utest" ROOT_PROMPT);
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, ROOT_PROMPT);
	break;
    case EVENT_BACKSP:
	break;
    case 'm':
	change_state(context, &menu_meas);
	break;
    case 't':
	change_state(context, &menu_timer);
	break;
    case 's':
	change_state(context, &menu_settings);
	break;
    case 'v':
	show_version();
	break;
    case 'u':
	change_state(context, menu_utest);
    default:
	invalid_character(context);
	break;
    }
}

void menu_timer(uint8_t event, context_t *context)
{
#define TIMER_PROMPT "\nRT>"

    reset_action(context);
    
    switch (event) {
    case EVENT_HELP:
	buffer_str(&gMnuBuffer, "\n\nActivate Show Begin End" TIMER_PROMPT);
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, TIMER_PROMPT);
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'a':
	context->action = act_timer_activate;
	change_state(context, &arg_timer);
	break;
    case 's':
	buffer_nl(&gMnuBuffer);
	sche_show(1);
	gFuncMLine = sche_show;
	break;
    case 'b':
	context->action = act_timer_begin;
	change_state(context, &arg_timer);
	break;
    case 'e':
	context->action = act_timer_end;
	change_state(context, &arg_timer);
	break;
    default:
	invalid_character(context);
	break;
    }
}

void menu_settings(uint8_t event, context_t *context)
{
#define SETTING_PROMPT "\nRS>"

    reset_action(context);

    switch (event) {
    case EVENT_HELP:
	buffer_str(&gMnuBuffer, "\n\nClock Debug" SETTING_PROMPT);
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, SETTING_PROMPT);
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'c':
	context->action = act_set_clock;
	change_state(context, arg_clock);
	break;
    case 'd':
	buffer_str(&gMnuBuffer, "\nDebug mode\n");
	break;
    default:
	invalid_character(context);
	break;
    }
}

void menu_meas(uint8_t event, context_t *context)
{
#define MEAS_PROMPT "\nRM>"

    reset_action(context);

    switch (event) {
    case EVENT_HELP:
	buffer_str(&gMnuBuffer, "\n\nShow Trigger Highlimit Lowlimit" MEAS_PROMPT);
	break;
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, MEAS_PROMPT);
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 's':
	buffer_nl(&gMnuBuffer);
	meas_show(1);
	gFuncMLine = meas_show;
	break;
    case 't':
	buffer_str(&gMnuBuffer, "\nmeasure trigger\n");
	break;
    case 'h':
	context->action = act_meas_high;
	change_state(context, &arg_meas);
	break;
    case 'l':
	context->action = act_meas_low;
	change_state(context, &arg_meas);
	break;
    default:
	invalid_character(context);
	break;
    }
}

void menu_utest(uint8_t event, context_t *context)
{
#define UTEST_PROMPT "\nRU>"

    reset_action(context);

    switch (event) {
    case EVENT_HELP:
	buffer_str(&gMnuBuffer, "\n\nAtest Btest" UTEST_PROMPT);
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, UTEST_PROMPT);
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'a':
	change_state(context, menu_root);
	break;
    case 'b':
	change_state(context, menu_root);
	break;
    default:
	invalid_character(context);
	break;
    }
}


void arg_timer(uint8_t event, context_t *context)
{
#define ATIMER_PROMPT "\nTIM>"

    if (event == EVENT_HELP) {
        buffer_nl(&gMnuBuffer);
	buffer_ch(&gMnuBuffer, TIMER_BEGIN);
	buffer_str(&gMnuBuffer, " .. ");
	buffer_ch(&gMnuBuffer, TIMER_END);
	buffer_str(&gMnuBuffer, ATIMER_PROMPT);
    }
    else if (event == EVENT_PROMPT) {
	buffer_str(&gMnuBuffer, ATIMER_PROMPT);
    }
    else if (event == EVENT_BACKSP) {
	change_state(context, &menu_timer);
    }
    else if ((event >= TIMER_BEGIN) && (event <= TIMER_END)) {
	add_arg(context, event - TIMER_BEGIN);
	buffer_ch(&gMnuBuffer, event);

	if ((context->action == act_timer_begin) ||
	    (context->action == act_timer_end))
	{
	    change_state(context, arg_clock);
	}
	else if (context->action == act_timer_activate) {
	    change_state(context, &arg_on_off);
	}
	else {
	    change_state(context, &menu_root);
	}
    }
    else {
	invalid_character(context);
    }
}


void arg_meas(uint8_t event, context_t *context)
{
#define AMEAS_PROMPT "\nMEA>"
    
    if (event == EVENT_HELP) {
        buffer_nl(&gMnuBuffer);
	buffer_ch(&gMnuBuffer, MEAS_BEGIN);
	buffer_str(&gMnuBuffer, " .. ");
	buffer_ch(&gMnuBuffer, MEAS_END);
	buffer_str(&gMnuBuffer, AMEAS_PROMPT);
    }
    else if (event == EVENT_PROMPT) {
	buffer_str(&gMnuBuffer, AMEAS_PROMPT);
    }
    else if (event == EVENT_BACKSP) {
	change_state(context, &menu_meas);
    }
    else if ((event >= MEAS_BEGIN) && (event <= MEAS_END)) {
	add_arg(context, event - MEAS_BEGIN);
	buffer_ch(&gMnuBuffer, event);

	if ((context->action == act_meas_high) ||
	    (context->action == act_meas_low))
	{
	    change_state(context, arg_value);
	}
	else {
	    change_state(context, menu_root);
	}
    }
    else {
	invalid_character(context);
    }
}


void arg_on_off(uint8_t event, context_t *context)
{
#define AONOFF_PROMPT "\nSTA>"
    
    switch (event) {
    case EVENT_HELP:
    case EVENT_PROMPT:
	buffer_str(&gMnuBuffer, AONOFF_PROMPT);
	break;
    case EVENT_BACKSP:
	if (context->action == act_timer_activate) {
	    change_state(context, arg_timer);
	}
	else {
	    change_state(context, menu_root);
	}
	break;
    case '0':
    case '1':
	add_arg(context, event - '0');

	if (context->action == act_timer_activate) {
	    execute(context);
	}
	change_state(context, menu_root);
	break;
    default:
	invalid_character(context);
	break;
    }
}

void arg_clock(uint8_t event, context_t *context)
{
    if ((event == EVENT_PROMPT) || 
	(event == EVENT_HELP)) 
    {
	buffer_str(&gMnuBuffer, "\nCLK>");
    }
    else if (event == EVENT_BACKSP) {
	if (context->substate > 0) {
	    context->substate--;
	    context->currentArg--;
	}
	else {
	    
	}
    }
    else if ((context->substate == 0 && event >= '0' && event <= '2') ||
	     (context->substate == 1 && event >= '0' && event <= '9') ||
	     (context->substate == 2 && event >= '0' && event <= '5') ||
	     (context->substate == 3 && event >= '0' && event <= '9'))
    {
	buffer_ch(&gMnuBuffer, event);
	add_arg(context, event - '0');
	(context->substate)++;
	if (context->substate == 4) {
	    execute(context);
	    change_state(context, menu_root);
	}
    }
    else {
	invalid_character(context);
    }	
}


void arg_value(uint8_t event, context_t *context)
{
    if ((event == EVENT_HELP) || 
	(event == EVENT_PROMPT)) 
    {
	buffer_str(&gMnuBuffer, "\nVAL>");
    }
    else if (event == EVENT_BACKSP) {
	if (context->substate > 0) {
	    context->substate--;
	    context->currentArg--;
	}
	else {
	    
	}
    }
    else if ((context->substate == 0 && event >= '0' && event <= '9') ||
	     (context->substate == 1 && event >= '0' && event <= '9') ||
	     (context->substate == 2 && event >= '0' && event <= '9'))
    {
	buffer_ch(&gMnuBuffer, event);
	add_arg(context, event - '0');
	(context->substate)++;
	if (context->substate == 3) {
	    execute(context);
	    change_state(context, menu_root);
	}
    }
    else {
	invalid_character(context);
    }
}
