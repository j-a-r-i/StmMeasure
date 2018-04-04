#ifndef __HW_H
#define __HW_H

#define UART 1

#ifdef stm32f4
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_tim.h"
#endif

#ifdef stm32f7
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_tim.h"
#endif

#ifdef stm32f0
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_usart.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_cortex.h"
#endif

#include "hal.h"
#include "rfm12b.h"

/** Event ids
 */
#define EV_TIMER2   0x01
#define EV_UART1_RX 0x02
#define EV_UART2_RX 0x04
#define EV_SPI1_RX  0x08
#define EV_SPI2_RX  0x10

/** Error codes
 */
typedef enum errors {
    ERR_INVALID_PIN = 0x01,
    ERR_UART_PORT,
    ERR_SPI_PORT,
    ERR_HARD_FAULT,
    ERR_INVALID_MEAS,
    ERR_INVALID_SCHE
} error_t;

extern rfm12b rfm1;
extern rfm12b rfm2;

extern uint32_t gEvents;
extern uint8_t  gUart1Rx;
extern uint8_t  gUart2Rx;
extern uint8_t  gSpi1Rx;
extern uint8_t  gSpi2Rx;

extern void error(error_t code);

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

#endif
