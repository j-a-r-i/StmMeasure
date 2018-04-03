#pragma once

#define UART 1

#include <stdio.h>
#include <stdint.h>

#include "hal.h"


/** Error codes
 */
typedef enum errors {
    ERR_INVALID_PIN = 0x01,
    ERR_UART_PORT,
    ERR_SPI_PORT,
    ERR_HARD_FAULT,
} error_t;

extern void error(error_t code);

