#pragma once

#include "ds1820.h"
#include "buffer.h"

typedef struct {
    uint16_t value;
    uint16_t highLimit;
    uint16_t lowLimit;
    uint8_t  romCode[DS1820_ROM_SIZE];
} meas_t;

#define MAX_MEASURE 5

extern meas_t gMeasureTable[MAX_MEASURE];

extern uint8_t meas_show(uint8_t meas, buffer_t *buf);
extern void meas_set_high_limit(uint8_t meas, uint16_t val);
extern void meas_set_low_limit(uint8_t meas, uint16_t val);
