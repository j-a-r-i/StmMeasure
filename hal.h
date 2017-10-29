#ifndef __HAL_H
#define __HAL_H

/* STM32F0 */
#define set_LED1 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define set_LED2 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9)


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

#define PIN_LED1    0
#define PIN_LED2    1
#define PIN_DS1820a 2
#define PIN_DS1820b 3
#define PIN_DS1820c 4

#define IO_MODE_INPUT  0
#define IO_MODE_OUTPUT 1

#define TRUE 1
#define FALSE 0

extern void delay_us(uint16_t time);

extern void io_init();
extern void io_mode(uint8_t pin, uint8_t mode);
extern void io_set(uint8_t pin);
extern void io_clear(uint8_t pin);
extern uint8_t io_read(uint8_t pin);

extern void uart_init();
extern void uart_send(char ch);
extern void uart_sends(char *buf);

extern void timer2_init();

#endif
