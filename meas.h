#pragma once

typedef struct {
    uint16_t value;
    uint16_t highLimit;
    uint16_t lowLimit;
    uint8_t  romCode[8];
} meas_t;

#define MAX_MEASURE 5

extern meas_t gMeasureTable[MAX_MEASURE];

extern void meas_show();
extern void meas_set_high_limit(uint8_t meas, uint16_t val);
extern void meas_set_low_limit(uint8_t meas, uint16_t val);
