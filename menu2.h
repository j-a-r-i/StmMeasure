#pragma once

struct context_s;

#define MAX_ARG 6
#define EVENT_HELP   '?'
#define EVENT_PROMPT 0xFF

typedef struct context_s {
    void (*state)(uint8_t, struct context_s*);
    uint8_t substate;
    void (*action)(uint8_t, uint8_t*);
    uint8_t args[MAX_ARG];
    uint8_t currentArg;
} context_t;

typedef void (*state_func)(uint8_t, context_t*);

extern void menu_root(uint8_t event, context_t *context);
