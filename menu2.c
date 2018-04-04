#include "hw.h"
#include "menu2.h"
#include "meas.h"
#include "scheduler.h"

#define NULL (void*)0

void act_timer_activate(uint8_t len, uint8_t *args);
void act_timer_begin(uint8_t len, uint8_t *args);
void act_timer_end(uint8_t len, uint8_t *args);
void act_meas_high(uint8_t len, uint8_t *args);
void act_meas_low(uint8_t len, uint8_t *args);
void act_set_clock(uint8_t len, uint8_t *args);
 
void menu_meas(uint8_t cmd, context_t *context);
void menu_timer(uint8_t cmd, context_t *context);
void menu_settings(uint8_t cmd, context_t *context);
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
    uart_sends(UART, "\ninvalid character\n");
    (*(context->state))(EVENT_PROMPT, context);
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

    uart_nl(UART);
    (*state)(EVENT_HELP, context);
    (*state)(EVENT_PROMPT, context);
}

void add_arg(context_t *context, uint8_t value)
{
    if (context->currentArg < MAX_ARG) {
	context->args[context->currentArg] = value;
	(context->currentArg)++;
    }
    else {
	uart_sends(UART, "\nerror: too many argumets!\n");
    }	
}

void execute(context_t *context)
{
    if (context->action != NULL)
	(*(context->action))(context->currentArg, context->args);
}

//------------------------------------------------------------------------------

void dump_args(const char* info, uint8_t len, uint8_t *args)
{
    uint8_t i;

    for (i=0; i<len; i++) {
		uart_send(UART, ' ');
		uart_hex8(UART, args[i]);
    }
    uart_nl(UART);
}

void act_timer_activate(uint8_t len, uint8_t *args)
{
    dump_args("timer act", len, args);
}

void act_timer_begin(uint8_t len, uint8_t *args)
{
    dump_args("timer begin", len, args);
    sche_set_start(args[0],
		   args[1]*10 + args[2],
		   args[3]*10 + args[4]);
}

void act_timer_end(uint8_t len, uint8_t *args)
{
    dump_args("timer end", len, args);
    sche_set_stop(args[0],
		  args[1]*10 + args[2],
		  args[3]*10 + args[4]);
}

void act_set_clock(uint8_t len, uint8_t *args)
{
    dump_args("set clock", len, args);
}

void act_meas_high(uint8_t len, uint8_t *args)
{
    dump_args("meas high", len, args);
    meas_set_high_limit(args[0], args[1]*100 + args[2]*10 + args[3]);
}

void act_meas_low(uint8_t len, uint8_t *args)
{
    dump_args("meas low", len, args);
    meas_set_low_limit(args[0], args[1]*100 + args[2]*10 + args[3]);
}

//------------------------------------------------------------------------------
void menu_root(uint8_t event, context_t *context)
{
    switch (event) {
    case EVENT_HELP:
	uart_sends(UART, "\n<m>eas | <t>imer | <s>ettings\n");
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	uart_sends(UART, "R>");
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
    default:
	invalid_character(context);
	break;
    }
}

void menu_timer(uint8_t event, context_t *context)
{
    reset_action(context);
    
    switch (event) {
    case EVENT_HELP:
	uart_sends(UART, "\n<a>ctivate | <s>how | <b>egin | <e>nd\n");
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	uart_sends(UART, "RT>");
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'a':
	context->action = act_timer_activate;
	change_state(context, &arg_timer);
	break;
    case 's':
	sche_show();
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
    reset_action(context);

    switch (event) {
    case EVENT_HELP:
	uart_sends(UART, "\n<c>lock | <d>ebug\n");
	break;
    case EVENT_ENTER:
    case EVENT_PROMPT:
	uart_sends(UART, "RS>");
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'c':
	context->action = act_set_clock;
	change_state(context, arg_clock);
	break;
    case 'd':
	uart_sends(UART, "\nDebug mode\n");
	break;
    default:
	invalid_character(context);
	break;
    }
}

void menu_meas(uint8_t event, context_t *context)
{
    reset_action(context);

    switch (event) {
    case EVENT_HELP:
	uart_sends(UART, "\n<s>how | <t>rigger | <h>igh limit | <l>ow limit\n");
	break;
    case EVENT_PROMPT:
	uart_sends(UART, "RM>");
	break;
    case EVENT_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 's':
	uart_nl(UART);
	meas_show();
	//uart_sends(UART, "\nmeasure show\n");
	break;
    case 't':
	uart_sends(UART, "\nmeasure trigger\n");
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

void arg_timer(uint8_t event, context_t *context)
{    
    if (event == EVENT_HELP) {
	uart_send(UART, TIMER_BEGIN);
	uart_sends(UART, " .. ");
	uart_send(UART, TIMER_END);
	uart_nl(UART);
    }
    else if (event == EVENT_PROMPT) {
	uart_sends(UART, "TIMER>");
    }
    else if (event == EVENT_BACKSP) {
	change_state(context, &menu_timer);
    }
    else if ((event >= TIMER_BEGIN) && (event <= TIMER_END)) {
	add_arg(context, event - TIMER_BEGIN);

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
    if (event == EVENT_HELP) {
        uart_nl(UART);
	uart_send(UART, MEAS_BEGIN);
	uart_sends(UART, " .. ");
	uart_send(UART, MEAS_END);
	uart_nl(UART);
    }
    else if (event == EVENT_PROMPT) {
	uart_sends(UART, "MEAS>");
    }
    else if (event == EVENT_BACKSP) {
	change_state(context, &menu_meas);
    }
    else if ((event >= MEAS_BEGIN) && (event <= MEAS_END)) {
	add_arg(context, event - MEAS_BEGIN);

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
    switch (event) {
    case EVENT_HELP:
	uart_sends(UART, "\n0 | 1\n");
	break;
    case EVENT_PROMPT:
	uart_sends(UART, "STATE>");
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
    if (event == EVENT_HELP) {
	uart_sends(UART, "\n0..9\n");
    }
    else if (event == EVENT_PROMPT) {
	uart_sends(UART, "CLOCK(");
	uart_send(UART, 'a' + context->args[0]);
	uart_sends(UART, ")>");
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
	uart_send(UART, event);
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
    if (event == EVENT_HELP) {
	uart_sends(UART, "\n0..9\n");
    }
    else if (event == EVENT_PROMPT) {
	uart_sends(UART, "VALUE(");
	uart_send(UART, 'a' + context->args[0]);
	uart_sends(UART, ")>");
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
	uart_send(UART, event);
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
