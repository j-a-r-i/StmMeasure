#include "hw.h"
#include "hal.h"
#include "event.h"

#define NULL (void*)0

typedef enum PinMode {
    PIN_OUTPUT,
    PIN_OUTPUT_HIGH,  // output with initial value high
    PIN_INPUT,
    PIN_AF_PULL_UP,
    PIN_AF_PULL_DOWN,
} pinmode_t;

typedef struct IoPinTable {
    GPIO_TypeDef *port;
    int pin;
    pinmode_t mode;
    uint32_t  alt;
} IoPinTable_t;

IoPinTable_t PINMAP[] = {
    [PIN_LED1]       = { GPIOC, LL_GPIO_PIN_8,  PIN_OUTPUT,       0 },
    [PIN_LED2]       = { GPIOC, LL_GPIO_PIN_9,  PIN_OUTPUT,       0 },
    [PIN_DS1820a]    = { GPIOB, LL_GPIO_PIN_3,  PIN_OUTPUT,       0 },
    [PIN_DS1820b]    = { GPIOB, LL_GPIO_PIN_4,  PIN_OUTPUT,       0 },
    [PIN_DS1820c]    = { GPIOB, LL_GPIO_PIN_5,  PIN_OUTPUT,       0 },
    [PIN_RFM12_SEL1] = { GPIOC, LL_GPIO_PIN_4,  PIN_OUTPUT_HIGH,  0 },
    [PIN_RFM12_IRQ1] = { GPIOA, LL_GPIO_PIN_4,  PIN_INPUT,        0 },
    [PIN_RFM12_SEL2] = { GPIOB, LL_GPIO_PIN_11, PIN_OUTPUT_HIGH,  0 },
    [PIN_RFM12_IRQ2] = { GPIOB, LL_GPIO_PIN_12, PIN_INPUT,        0 },

    [PIN_UART1_TX]   = { GPIOA, LL_GPIO_PIN_9,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
    [PIN_UART1_RX]   = { GPIOA, LL_GPIO_PIN_10, PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
    [PIN_UART2_TX]   = { GPIOA, LL_GPIO_PIN_2,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 },  // use this UART
    [PIN_UART2_RX]   = { GPIOA, LL_GPIO_PIN_3,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
    [PIN_SPI1_SCK]   = { GPIOA, LL_GPIO_PIN_5,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
    [PIN_SPI1_MISO]  = { GPIOA, LL_GPIO_PIN_6,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
    [PIN_SPI1_MOSI]  = { GPIOA, LL_GPIO_PIN_7,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
    [PIN_SPI2_SCK]   = { GPIOB, LL_GPIO_PIN_10, PIN_AF_PULL_DOWN, LL_GPIO_AF_5 },
    [PIN_SPI2_MISO]  = { GPIOC, LL_GPIO_PIN_2,  PIN_AF_PULL_DOWN, LL_GPIO_AF_1 },
    [PIN_SPI2_MOSI]  = { GPIOC, LL_GPIO_PIN_3,  PIN_AF_PULL_DOWN, LL_GPIO_AF_1 },

//  [PIN_UART1_TX]   = { GPIOA, LL_GPIO_PIN_9,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 }, //stm32f052
//  [PIN_UART1_RX]   = { GPIOA, LL_GPIO_PIN_10, PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
//  [PIN_UART2_TX]   = { GPIOA, LL_GPIO_PIN_2,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
//  [PIN_UART2_RX]   = { GPIOA, LL_GPIO_PIN_3,  PIN_AF_PULL_UP,   LL_GPIO_AF_1 },
//  [PIN_SPI1_SCK]   = { GPIOA, LL_GPIO_PIN_5,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
//  [PIN_SPI1_MISO]  = { GPIOA, LL_GPIO_PIN_6,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
//  [PIN_SPI1_MOSI]  = { GPIOA, LL_GPIO_PIN_7,  PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
//  [PIN_SPI2_SCK]   = { GPIOB, LL_GPIO_PIN_13, PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
//  [PIN_SPI2_MISO]  = { GPIOB, LL_GPIO_PIN_14, PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
//  [PIN_SPI2_MOSI]  = { GPIOB, LL_GPIO_PIN_15, PIN_AF_PULL_DOWN, LL_GPIO_AF_0 },
};

/* for STM32F4
    [PIN_LED1]       = { GPIOD, LL_GPIO_PIN_13, PIN_OUTPUT,       0 },
    [PIN_LED2]       = { GPIOD, LL_GPIO_PIN_12, PIN_OUTPUT,       0 },
    [PIN_LED3]       = { GPIOD, LL_GPIO_PIN_14, PIN_OUTPUT,       0 },
    [PIN_LED4]       = { GPIOD, LL_GPIO_PIN_15, PIN_OUTPUT,       0 },
    [PIN_UART1_TX]   = { GPIOA, LL_GPIO_PIN_2,  PIN_AF_PULL_UP,   LL_GPIO_AF_7 },
    [PIN_UART1_RX]   = { GPIOA, LL_GPIO_PIN_3,  PIN_AF_PULL_UP,   LL_GPIO_AF_7 },
    [PIN_SPI1_SCK]   = { GPIOB, LL_GPIO_PIN_3,  PIN_AF_PULL_DOWN, LL_GPIO_AF_5 },
    [PIN_SPI1_MISO]  = { GPIOB, LL_GPIO_PIN_4,  PIN_AF_PULL_DOWN, LL_GPIO_AF_5 },
    [PIN_SPI1_MOSI]  = { GPIOB, LL_GPIO_PIN_5,  PIN_AF_PULL_DOWN, LL_GPIO_AF_5 },
*/

/* for STM32F7
    [PIN_LED1]       = { GPIOB, LL_GPIO_PIN_0,  PIN_OUTPUT,       0 },
    [PIN_LED2]       = { GPIOB, LL_GPIO_PIN_7,  PIN_OUTPUT,       0 },
    [PIN_LED3]       = { GPIOB, LL_GPIO_PIN_14, PIN_OUTPUT,       0 },
    [PIN_UART1_TX]   = { GPIOD, LL_GPIO_PIN_8,  PIN_AF_PULL_UP,   LL_GPIO_AF_7 },
    [PIN_UART1_RX]   = { GPIOD, LL_GPIO_PIN_9,  PIN_AF_PULL_UP,   LL_GPIO_AF_7 },
*/
#define MAX_PINMAP sizeof PINMAP / sizeof PINMAP[0]


//------------------------------------------------------------------------------
void delay_us(uint16_t time)
{
    volatile uint16_t counter = 2 * time;
    while(counter--) {
	asm("nop");
    }
}

void pin_alt(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t alternate, uint32_t pull)
{
    LL_GPIO_SetPinMode(GPIOx, pin, LL_GPIO_MODE_ALTERNATE);
    if (pin < LL_GPIO_PIN_8)
	LL_GPIO_SetAFPin_0_7(GPIOx, pin, alternate);
    else
	LL_GPIO_SetAFPin_8_15(GPIOx, pin, alternate);

    if (pull == LL_GPIO_PULL_UP)
	LL_GPIO_SetPinOutputType(GPIOx, pin, LL_GPIO_OUTPUT_PUSHPULL);

    LL_GPIO_SetPinSpeed(GPIOx, pin, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOx, pin, pull);
}

//------------------------------------------------------------------------------
void io_init()
{
    uint8_t i;

#ifdef stm32f0
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
#endif
#ifdef stm32f4
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
#endif
#ifdef stm32f7
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
#endif

    for (i=0; i<PIN_MAX; i++) {
	switch (PINMAP[i].mode) {

	case PIN_OUTPUT_HIGH:
	case PIN_OUTPUT:
	    LL_GPIO_SetPinMode(PINMAP[i].port,
			       PINMAP[i].pin,
			       LL_GPIO_MODE_OUTPUT);
	    if (PINMAP[i].mode == PIN_OUTPUT_HIGH) {
		LL_GPIO_SetOutputPin(PINMAP[i].port,
				     PINMAP[i].pin);
	    }
	    break;

	case PIN_INPUT:
	    LL_GPIO_SetPinMode(PINMAP[i].port,
			       PINMAP[i].pin,
			       LL_GPIO_MODE_INPUT);
	    break;

	case PIN_AF_PULL_DOWN:
	    pin_alt(PINMAP[i].port,
		    PINMAP[i].pin,
		    PINMAP[i].alt,
		    LL_GPIO_PULL_DOWN);
	    break;

	case PIN_AF_PULL_UP:
	    pin_alt(PINMAP[i].port,
		    PINMAP[i].pin,
		    PINMAP[i].alt,
		    LL_GPIO_PULL_UP);
	    break;
	}
    }     
}

void io_mode(pin_t pin, uint8_t mode)
{
    if (mode == IO_MODE_INPUT) {
	LL_GPIO_SetPinMode(PINMAP[pin].port,
			   PINMAP[pin].pin,
			   LL_GPIO_MODE_INPUT);
    }
    else {
	LL_GPIO_SetPinMode(PINMAP[pin].port,
			   PINMAP[pin].pin,
			   LL_GPIO_MODE_OUTPUT);
    }
}

void io_set(pin_t pin)
{
    LL_GPIO_SetOutputPin(PINMAP[pin].port, PINMAP[pin].pin);
}

void io_clear(pin_t pin)
{
    LL_GPIO_ResetOutputPin(PINMAP[pin].port, PINMAP[pin].pin);
}

uint8_t io_read(pin_t pin)
{
    return LL_GPIO_IsInputPinSet(PINMAP[pin].port, PINMAP[pin].pin);
}


//------------------------------------------------------------------------------
void SPI1_IRQHandler()
{
    uint8_t data;
    
    if (LL_SPI_IsActiveFlag_RXNE(SPI1) && LL_SPI_IsEnabledIT_RXNE(SPI1)) {
	/* RXNE flag will be cleared by reading of RDR register (done in call) */
	/* Call function in charge of handling Character reception */
	//SPI_CharReception_Callback();
	data  = LL_SPI_ReceiveData16(SPI1);

	EVENT_SET(EV_SPI1_RX, data);
    }
}

void SPI2_IRQHandler()
{
    uint8_t data;
    if (LL_SPI_IsActiveFlag_RXNE(SPI2) && LL_SPI_IsEnabledIT_RXNE(SPI2)) {
	/* RXNE flag will be cleared by reading of RDR register (done in call) */
	/* Call function in charge of handling Character reception */
	//SPI_CharReception_Callback();
	data = LL_SPI_ReceiveData16(SPI2);

	EVENT_SET(EV_SPI2_RX, data);
    }
}

//------------------------------------------------------------------------------
SPI_TypeDef *spi_port(uint8_t port)
{
    SPI_TypeDef *ret = NULL;

    switch (port) {
    case 1:
	ret = SPI1;
	break;
    case 2:
	ret = SPI2;
	break;
    default:
	error(ERR_SPI_PORT);
	break;
    }
    return ret;
}


//------------------------------------------------------------------------------
void spi_init(uint8_t port)
{
    SPI_TypeDef *spi = spi_port(port);

    switch (port) {
    case 1:
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);
	break;
    case 2:
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
	break;
    }

    LL_SPI_SetMode(spi, LL_SPI_MODE_MASTER);
    LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
    LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
    LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV256);
    LL_SPI_SetTransferBitOrder(spi, LL_SPI_MSB_FIRST);
    LL_SPI_SetTransferDirection(spi, LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(spi, LL_SPI_DATAWIDTH_16BIT);
    LL_SPI_SetNSSMode(spi, LL_SPI_NSS_SOFT);
    
    LL_SPI_Enable(spi);
}

uint16_t spi_write(uint8_t port, uint16_t data)
{
    SPI_TypeDef *spi = spi_port(port);

    LL_SPI_TransmitData16(spi, data);

    while (LL_SPI_IsActiveFlag_BSY(spi))
	;

    return LL_SPI_ReceiveData16(spi);    
}

//------------------------------------------------------------------------------
/** Hard Fault exception
 */
void HardFault_Handler(void)
{
    error(ERR_HARD_FAULT);
}
