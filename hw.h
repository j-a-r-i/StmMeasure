#pragma once

#define UART 2

#ifdef stm32f4
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_utils.h"
#endif

#ifdef stm32f7
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_utils.h"
#endif

#ifdef stm32f0
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_cortex.h"
#endif

#include <stdint.h>
#include "hal.h"
#include "rfm12b.h"
#include "buffer.h"

#define NULL (void*)0


/** Error codes
 */
typedef enum errors {
    ERR_HARD_FAULT         = 0x01,  // CPU fault

    ERR_UART_PARITY,        // 02
    ERR_UART_FRAMING,       // 03
    ERR_UART_NOISE,         // 04
    ERR_UART_OVERRUN,       // 05

    ERR_INVALID_PIN,        // 06
    ERR_UART_PORT,          // 07
    ERR_SPI_PORT,           // 08
    ERR_INVALID_MEAS,       // 09
    ERR_INVALID_SCHE,       // 0A
    ERR_BUFFER_OVERFLOW,    // 0B
    ERR_ARG_COUNT,          // 0C
    ERR_INVALID_COMMAND,    // 0D
    ERR_SYNTAX,             // 0E
    ERR_ARGUMENT            // 0F
} error_t;

/** Function pointer for multiline printing,
 *  when there is more printing that buffer_t
 *  can hold.
 */
typedef uint8_t (*func_mline)(uint8_t first, buffer_t *buf);

extern rfm12b rfm1;
extern rfm12b rfm2;
extern const char *gVersion;

extern func_mline gFuncMLine;

extern void _error(error_t code);
extern void show_version();

/*
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_adc.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_crc.h"
#include "stm32f4xx_ll_dac.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_dma2d.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_iwdg.h"
#include "stm32f4xx_ll_rtc.h"
#include "stm32f4xx_ll_wwdg.h"
#include "stm32f4xx_ll_rng.h"
#include "stm32f4xx_ll_lptim.h"
*/
