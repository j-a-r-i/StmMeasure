#pragma once

#define UART 1

#include <stdio.h>
#include <stdint.h>

#include "hal.h"
#include "buffer.h"

/** Event ids
 */
#define EV_UART1_RX 0x02
#define EV_UART1_TX 0x04

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

extern uint32_t gEvents;
extern uint8_t  gUart1Rx;
extern func_mline gFuncMLine;


extern void error(error_t code);
extern void trace(uint8_t ch);
extern void buffer_print(buffer_t*);
