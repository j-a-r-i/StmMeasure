#include "hw.h"
#include "menu2.h"

extern uint8_t event_loop();


//------------------------------------------------------------------------------
void main(int argc, char **argv)
{
    uint8_t ch;
    context_t context;

    context.state = &menu_root;
    context.action = NULL;

    (*(context.state))(EVENT_PROMPT, &context);
    
    while (1) {
	ch = event_loop();

	(*(context.state))(ch, &context);
	if (ch == EVENT_HELP) {
	    uart_nl(UART);
	    (*(context.state))(EVENT_PROMPT, &context);
	}
    }
}
