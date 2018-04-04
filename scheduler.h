#pragma once

typedef struct {
    uint8_t wday:3;
    uint8_t hour:5;
    uint8_t min:6;
    uint8_t enabled:1;
    uint8_t handled:1;
} __attribute__((packed)) time_t;


typedef struct {
    uint8_t days;
    time_t  start;
    time_t  stop;
    uint8_t flags;
    uint8_t action;
} sched_t;

// day bits
//
#define DAY_MON  0x01
#define DAY_TUE  0x02
#define DAY_WED  0x04
#define DAY_THU  0x08
#define DAY_FRI  0x10
#define DAY_SAT  0x20
#define DAY_SUN  0x40
#define DAY_ALL  0x7F

#define MAX_SCHEDULE 10


extern void sche_show();
extern void sche_set_start(uint8_t timer, uint8_t hour, uint8_t min);
extern void sche_set_stop(uint8_t timer, uint8_t hour, uint8_t min);
						  
