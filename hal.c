#include "hw.h"
#include "hal.h"

uint8_t gUartRx;

//------------------------------------------------------------------------------
void delay_us(uint16_t time)
{
}

//------------------------------------------------------------------------------
void io_init()
{
#ifdef stm32f4
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    // UART TX
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);

    // UART RX
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    // SPI SCK
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_DOWN);

    // SPI MISO
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_4, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_DOWN);

    // SPI MOSI
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_5, LL_GPIO_AF_5);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);
#endif

#ifdef stm32f7
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

    // UART TX
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_8, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

    // UART RX
    LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_9, LL_GPIO_AF_7);
    LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
    LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
#endif
}

#ifdef stm32f4
#define UART_PORT USART2
#endif
#ifdef stm32f7
#define UART_PORT ((USART_TypeDef *) USART3_BASE)
#endif

//------------------------------------------------------------------------------
//void USARTx_IRQHandler()
void USART3_IRQHandler()
{
  if (LL_USART_IsActiveFlag_RXNE(UART_PORT) && LL_USART_IsEnabledIT_RXNE(UART_PORT)) {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
    //USART_CharReception_Callback();
      gUartRx = LL_USART_ReceiveData8(UART_PORT);

      //uart_send(ch);
      
      gEvents |= EV_UART_RX;
  }
}


//------------------------------------------------------------------------------
void uart_init()
{
#ifdef stm32f0
    NVIC_SetPriority(USARTx_IRQn, 0);  
    NVIC_EnableIRQ(USARTx_IRQn);
#endif
#ifdef stm32f4
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
#endif
#ifdef stm32f7
    NVIC_SetPriority(USART3_IRQn, 0);  
    NVIC_EnableIRQ(USART3_IRQn);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
#endif

    LL_USART_SetTransferDirection(UART_PORT,
				  LL_USART_DIRECTION_TX_RX);

    LL_USART_ConfigCharacter(UART_PORT,
			     LL_USART_DATAWIDTH_8B,
			     LL_USART_PARITY_NONE,
			     LL_USART_STOPBITS_1);

    LL_USART_SetBaudRate(UART_PORT,
			 SystemCoreClock/2,
			 LL_USART_OVERSAMPLING_16,
//			 9600);
			 115200); 

    LL_USART_Enable(UART_PORT);

    LL_USART_EnableIT_RXNE(UART_PORT);
    LL_USART_EnableIT_ERROR(UART_PORT);
}

//------------------------------------------------------------------------------
void uart_send(char ch)
{
    LL_USART_TransmitData8(UART_PORT, ch);
}

//------------------------------------------------------------------------------
void uart_sends(char *buf)
{
    for (uint8_t i = 0; buf[i] != 0; i++) {
	while (!LL_USART_IsActiveFlag_TXE(UART_PORT));
	
	LL_USART_TransmitData8(UART_PORT, buf[i]);	
    }
}


//------------------------------------------------------------------------------
void TIM2_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1) {
	LL_TIM_ClearFlag_UPDATE(TIM2);
    }

    //LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_14);
    gEvents |= EV_TIMER2;
}

void timer2_init()
{
    uint32_t clock, reload;
    
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 10000));

    clock = SystemCoreClock/1;

    reload = __LL_TIM_CALC_ARR(clock, LL_TIM_GetPrescaler(TIM2), 1);
    LL_TIM_SetAutoReload(TIM2, reload);

    LL_TIM_EnableIT_UPDATE(TIM2);
    
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);

    LL_TIM_EnableCounter(TIM2);
    LL_TIM_GenerateEvent_UPDATE(TIM2);
}


//------------------------------------------------------------------------------
/** Hard Fault exception
 */
void HardFault_Handler(void)
{
    error(0);
}
