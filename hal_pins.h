#pragma once

/* Application dependent part of HAL.
 * Every application got different pins.
 */

/* STM32F0 */
#define set_LED1 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define tgl_LED1 LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8)

#define set_LED2 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9)
#define tgl_LED2 LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_9)

/* STM32F4
#define set_LED1 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13)
#define set_LED2 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12)
#define set_LED3 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14)
#define clr_LED3 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14)
#define set_LED4 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_15)
#define clr_LED4 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_15)
*/

/* STM32F7
#define set_LED1 LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0)
#define set_LED2 LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7)
#define set_LED3 LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14)
#define clr_LED3 LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14)
*/

typedef enum pin {
    PIN_LED1 = 0,
    PIN_LED2,
    PIN_DS1820a,
    PIN_DS1820b,
    PIN_DS1820c,
    PIN_RFM12_SEL1,
    PIN_RFM12_IRQ1,
    PIN_RFM12_SEL2,
    PIN_RFM12_IRQ2,
    PIN_MAX
} pin_t;
