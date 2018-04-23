#pragma once

typedef void (*func_event)(uint8_t arg);

typedef struct {
    uint8_t arg;
    func_event func;
} event_t;


typedef enum {
    EV_TIMER2 = 0,
    EV_UART1_RX,
    EV_UART1_TX,
    EV_UART2_RX,
    EV_UART2_TX,
    EV_USER1,
    EV_USER2,
    EV_MAX
} event_id_t;


extern uint32_t gEvents2;
extern event_t  gEvents2Table[];

#define EVENT_SET(ev,argument) gEvents2 |= (1<<ev); gEvents2Table[ev].arg = argument

extern void event_init();
extern void event_handle();
