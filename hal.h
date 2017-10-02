#ifndef __HAL_H
#define __HAL_H

#define set_LED1 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13)
#define set_LED2 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12)
#define set_LED3 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14)
#define clr_LED3 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14)
#define set_LED4 LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_15)
#define clr_LED4 LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_15)

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

#endif