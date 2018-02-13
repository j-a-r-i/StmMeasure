#ifndef __HAL_H
#define __HAL_H

/* STM32F0 */
#define set_LED1 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define clr_LED1 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8)
#define tgl_LED1 LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8)

#define set_LED2 LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9)
#define clr_LED2 LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9)
#define tgl_LED2 LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_9)

#define setRFM12_SEL LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define clrRFM12_SEL LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)
#define tglRFM12_SEL LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_4)

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
    PIN_LED1,
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

#define PIN_LED1    0
#define PIN_LED2    1
#define PIN_DS1820a 2
#define PIN_DS1820b 3
#define PIN_DS1820c 4
#define PIN_RFM12_SEL 5

#define IO_MODE_INPUT  0
#define IO_MODE_OUTPUT 1

#define TRUE 1
#define FALSE 0

extern void delay_us(uint16_t time);

extern void io_init();
extern void io_pin_init(pin_t pin, uint8_t mode);
extern void io_mode(pin_t, uint8_t mode);
extern void io_set(pin_t pin);
extern void io_clear(pin_t pin);
extern uint8_t io_read(pin_t pin);

extern void uart_init(uint8_t port);
extern void uart_send(uint8_t port, char ch);
extern void uart_sends(uint8_t port, char *buf);
extern void uart_send_nl(uint8_t port);

extern void timer2_init();

extern void spi_init(uint8_t port);
extern void spi_write(uint8_t port, uint16_t data);
extern uint16_t spi_read(uint8_t port);

#endif
