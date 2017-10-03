#ifndef __HW_H
#define __HW_H

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
#endif

#include "hal.h"

#define EV_TIMER2  0x01
#define EV_UART_RX 0x02

extern uint32_t gEvents;
extern uint8_t  gUartRx;

extern void error(uint8_t code);

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
