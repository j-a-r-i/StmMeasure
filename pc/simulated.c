#include "hw.h"
#include "menu.h"
#include "meas.h"
#include "scheduler.h"

extern uint8_t event_loop();

struct context_s;

#define MAX_ARG 6

typedef struct context_s {
    void (*state)(uint8_t, struct context_s*);
    uint8_t substate;
    void (*action)(uint8_t, uint8_t*);
    uint8_t args[MAX_ARG];
    uint8_t currentArg;
} context_t;

typedef void (*state_func)(uint8_t, context_t*);

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

#define CMD_HELP   '?'
#define CMD_PROMPT 0xFF
#define CMD_BACKSP 127

#define TIMER_BEGIN 'a'
#define TIMER_END   'a' + MAX_SCHEDULE
#define MEAS_BEGIN 'a'
#define MEAS_END   'a' + MAX_MEASURE

//------------------------------------------------------------------------------
void invalid_character(context_t *context)
{
    uart_sends(UART, "\ninvalid character\n");
    (*(context->state))(CMD_PROMPT, context);
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
    (*state)(CMD_HELP, context);
    (*state)(CMD_PROMPT, context);
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
}

void act_timer_end(uint8_t len, uint8_t *args)
{
    dump_args("timer end", len, args);
}

void act_set_clock(uint8_t len, uint8_t *args)
{
    dump_args("set clock", len, args);
}

void act_meas_high(uint8_t len, uint8_t *args)
{
    dump_args("alarm beg", len, args);
}

void act_meas_low(uint8_t len, uint8_t *args)
{
    dump_args("alarm end", len, args);
}

//------------------------------------------------------------------------------
void menu_root(uint8_t cmd, context_t *context)
{
    switch (cmd) {
    case CMD_HELP:
	uart_sends(UART, "<m>eas | <t>imer | <s>ettings\n");
	break;
    case CMD_PROMPT:
	uart_sends(UART, "R>");
	break;
    case CMD_BACKSP:
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
	printf("%d", cmd);
	invalid_character(context);
	break;
    }
}

void menu_timer(uint8_t cmd, context_t *context)
{
    reset_action(context);
    
    switch (cmd) {
    case CMD_HELP:
	uart_sends(UART, "<a>ctivate | <s>how | <b>egin | <e>nd\n");
	break;
    case CMD_PROMPT:
	uart_sends(UART, "RT>");
	break;
    case CMD_BACKSP:
	change_state(context, &menu_root);
	break;	
    case 'a':
	context->action = act_timer_activate;
	change_state(context, &arg_timer);
	break;
    case 's':
	uart_sends(UART, "\ntimer show\n");
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

void menu_settings(uint8_t cmd, context_t *context)
{
    reset_action(context);

    switch (cmd) {
    case CMD_HELP:
	uart_sends(UART, "<c>lock | <d>ebug\n");
	break;
    case CMD_PROMPT:
	uart_sends(UART, "RS>");
	break;
    case CMD_BACKSP:
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

void menu_meas(uint8_t cmd, context_t *context)
{
    reset_action(context);

    switch (cmd) {
    case CMD_HELP:
	uart_sends(UART, "<s>how | <t>rigger | <h>igh limit | <l>ow limit\n");
	break;
    case CMD_PROMPT:
	uart_sends(UART, "RM>");
	break;
    case CMD_BACKSP:
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

void arg_timer(uint8_t cmd, context_t *context)
{    
    if (cmd == CMD_HELP) {
	uart_send(UART, TIMER_BEGIN);
	uart_sends(UART, " .. ");
	uart_send(UART, TIMER_END);
	uart_nl(UART);
    }
    else if (cmd == CMD_PROMPT) {
	uart_sends(UART, "TIMER>");
    }
    else if (cmd == CMD_BACKSP) {
	change_state(context, &menu_timer);
    }
    else if ((cmd >= TIMER_BEGIN) && (cmd <= TIMER_END)) {
	add_arg(context, cmd - TIMER_BEGIN);

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


void arg_meas(uint8_t cmd, context_t *context)
{    
    if (cmd == CMD_HELP) {
	uart_send(UART, MEAS_BEGIN);
	uart_sends(UART, " .. ");
	uart_send(UART, MEAS_END);
	uart_nl(UART);
    }
    else if (cmd == CMD_PROMPT) {
	uart_sends(UART, "MEAS>");
    }
    else if (cmd == CMD_BACKSP) {
	change_state(context, &menu_meas);
    }
    else if ((cmd >= MEAS_BEGIN) && (cmd <= MEAS_END)) {
	add_arg(context, cmd - MEAS_BEGIN);

	if ((context->action == act_meas_high) ||
	    (context->action == act_meas_low))
	{
	    execute(context);
	    change_state(context, &menu_root);
	}
	else {
	    change_state(context, &menu_root);
	}
    }
    else {
	invalid_character(context);
    }
}


void arg_on_off(uint8_t cmd, context_t *context)
{
    switch (cmd) {
    case CMD_HELP:
	uart_sends(UART, "0 | 1\n");
	break;
    case CMD_PROMPT:
	uart_sends(UART, "STATE>");
	break;
    case CMD_BACKSP:
	if (context->action == act_timer_activate) {
	    change_state(context, arg_timer);
	}
	else {
	    change_state(context, menu_root);
	}
	break;
    case '0':
    case '1':
	add_arg(context, cmd - '0');

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

void arg_clock(uint8_t cmd, context_t *context)
{
    if (cmd == CMD_HELP) {
	uart_sends(UART, "0..9\n");
    }
    else if (cmd == CMD_PROMPT) {
	uart_sends(UART, "CLOCK>");
    }
    else if (cmd == CMD_BACKSP) {
	if (context->substate > 0) {
	    context->substate--;
	    context->currentArg--;
	}
	else {
	    
	}
    }
    else if ((context->substate == 0 && cmd >= '0' && cmd <= '2') ||
	     (context->substate == 1 && cmd >= '0' && cmd <= '9') ||
	     (context->substate == 2 && cmd >= '0' && cmd <= '5') ||
	     (context->substate == 3 && cmd >= '0' && cmd <= '9'))
    {
	uart_send(UART, cmd);
	add_arg(context, cmd - '0');
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


//------------------------------------------------------------------------------
void main(int argc, char **argv)
{
    uint8_t ch;
    context_t context;

    context.state = menu_root;
    context.action = NULL;

    (*(context.state))(CMD_PROMPT, &context);
    
    while (1) {
	ch = event_loop();

	if (ch == CMD_HELP)
	    uart_nl(UART);
	
	(*(context.state))(ch, &context);
	if (ch == CMD_HELP) {
	    uart_nl(UART);
	    (*(context.state))(CMD_PROMPT, &context);
	}
    }
}
