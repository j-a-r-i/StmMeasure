#pragma once

#define UART 1

#include <stdio.h>
#include <stdint.h>

#include "hal.h"
#include "buffer.h"

/** Error codes
 */
typedef enum errors {
    ERR_INVALID_PIN = 0x01,
    ERR_UART_PORT,
    ERR_SPI_PORT,
    ERR_HARD_FAULT,
    ERR_BUFFER_OVERFLOW,
    ERR_INVALID_COMMAND,
    ERR_SYNTAX,
    ERR_ARGUMENT
} error_t;

/** Function pointer for multiline printing,
 *  when there is more printing that buffer_t
 *  can hold.
 */
typedef uint8_t (*func_mline)(uint8_t timer, buffer_t *buf);

extern func_mline gFuncMLine;


extern void error(error_t code);
extern void trace(uint8_t ch);
