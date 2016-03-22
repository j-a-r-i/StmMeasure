#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_conf.h"

#include "stm32f0xx_hal_rcc.h"
#include "stm32f0xx_hal_flash.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void clock_init(void);
void gpio_init(void);
void timer_init(void);
void uart_init(void);

static int ledDelay = 200;

static TIM_HandleTypeDef timer2Handle;  // HAL
static UART_HandleTypeDef uart1Handle;
static SPI_HandleTypeDef spi1Handle;

static QueueHandle_t commQueue1;  // FreeRTOS

#define NOTUSED(x) NOTUSED_ ## x __attribute__((__unused__))

#define LED_BLUE GPIO_PIN_8
#define LED_GREEN GPIO_PIN_9

//------------------------------------------------------------------------------
void AssertCalled(void)
{
    HAL_GPIO_WritePin(GPIOC, LED_GREEN, 1);
    HAL_GPIO_WritePin(GPIOC, LED_BLUE, 1);
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

//------------------------------------------------------------------------------
void TaskBlink(void* NOTUSED(arg))
{
    int stat = 0;
    char ch = ' ';
    
    HAL_GPIO_WritePin(GPIOC, LED_GREEN, 0);
    for(;;) {
        if (xQueueReceive(commQueue1, &ch, (TickType_t)1000)) {           
            HAL_GPIO_WritePin(GPIOC, LED_GREEN, stat);

            stat = !stat;
        }
        vTaskDelay(100);
    }
}

//------------------------------------------------------------------------------
void TaskMeasure(void* NOTUSED(arg))
{
    for(;;) {
        vTaskDelay(ledDelay);
        HAL_GPIO_WritePin(GPIOC, LED_BLUE, 1);
        vTaskDelay(ledDelay);
        HAL_GPIO_WritePin(GPIOC, LED_BLUE, 0);
    }
}

//------------------------------------------------------------------------------
void TaskButton(void*  NOTUSED(arg))
{
    uint8_t newstate, state = 1;
    for (;;) {
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) != state) {
            vTaskDelay(20);
            newstate = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
            if ( newstate != state ) {
                state = newstate;
                if (newstate) {
                    ledDelay += 400;
                    if (ledDelay > 1000)
                        ledDelay = 200;
                }
            }
        }
        vTaskDelay(20);
    }
}

//------------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uart)
{
    
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    
}

//------------------------------------------------------------------------------
void TaskTerminal(void*  NOTUSED(arg))
{
    static const char* msg = "Hello world\r\n";
    
    for (;;) {
        //HAL_UART_Transmit_IT(&uart1Handle, (uint8_t*)msg, 13);
      HAL_UART_Transmit(&uart1Handle, (uint8_t*)msg, 13, 10000);
      //HAL_GPIO_WritePin(GPIOC, LED_GREEN, 1);
      vTaskDelay(8000);
      //HAL_GPIO_WritePin(GPIOC, LED_GREEN, 0);
      //vTaskDelay(4000);
    }
}


//------------------------------------------------------------------------------
int main(void)
{
    TaskHandle_t h1, h2, h3, h4;

    HAL_Init();
    clock_init();

    // System interrupt init
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    // FreeRTOS
    commQueue1 = xQueueCreate(4, sizeof(char));



    xTaskCreate(TaskBlink,
                "blink",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                &h1);

    xTaskCreate(TaskMeasure,
                "meas",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                &h2);

    xTaskCreate(TaskButton,
                "btn",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                &h3);

    xTaskCreate(TaskTerminal,
                "term",
                configMINIMAL_STACK_SIZE,
                NULL,
                2,
                &h4);
    
    // Initialize all configured peripherals
    gpio_init();
    timer_init();
    uart_init();

    vTaskStartScheduler();
    
    HAL_GPIO_WritePin(GPIOC, LED_GREEN, 1);
    HAL_GPIO_WritePin(GPIOC, LED_BLUE, 1);
    while (1) {
    }
}

//------------------------------------------------------------------------------
/** Clock configuration
 */
void clock_init(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  //PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  __SYSCFG_CLK_ENABLE();
}

//------------------------------------------------------------------------------
void timer_init(void)
{
    __TIM2_CLK_ENABLE();
    timer2Handle.Instance = TIM2; 
    timer2Handle.Init.Prescaler = 271;
    timer2Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    timer2Handle.Init.Period = 62499;
    timer2Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&timer2Handle);     // Init timer
    
    HAL_TIM_Base_Start_IT(&timer2Handle); // start timer interrupts
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1); 
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

//------------------------------------------------------------------------------
void uart_init(void)
{
  uart1Handle.Instance = USART1;
  uart1Handle.Init.BaudRate = 9600;
  uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  uart1Handle.Init.StopBits = UART_STOPBITS_1;
  uart1Handle.Init.Parity = UART_PARITY_NONE;
  uart1Handle.Init.Mode = UART_MODE_TX_RX;
  uart1Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart1Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  uart1Handle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
  uart1Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&uart1Handle);

  spi1Handle.Instance = SPI1;
  spi1Handle.Init.Mode = SPI_MODE_SLAVE;
  spi1Handle.Init.Direction = SPI_DIRECTION_2LINES;
  spi1Handle.Init.DataSize = SPI_DATASIZE_8BIT;
  spi1Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
  spi1Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
  spi1Handle.Init.NSS = SPI_NSS_SOFT;
  spi1Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  spi1Handle.Init.TIMode = SPI_TIMODE_DISABLED;
  spi1Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  spi1Handle.Init.CRCPolynomial = 7;
  spi1Handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  spi1Handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLED;
  HAL_SPI_Init(&spi1Handle);
  
}

//------------------------------------------------------------------------------
/** input/output configuration.
 */
void gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC8 PC9 */
  /**
   * PC8 - LD4 [blue led]
   * PC9 - LD3 [green led]
   */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

#define LED_BLUE GPIO_PIN_8
#define LED_GREEN GPIO_PIN_9

extern void xPortSysTickHandler(void);

//------------------------------------------------------------------------------
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
    HAL_IncTick();
}

//------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	char cIn = 'a';
	BaseType_t highPriorityTaskWoken = pdFALSE;

	if (htim == &timer2Handle) {
        if (commQueue1 != NULL) {
        	xQueueSendToBackFromISR(commQueue1, &cIn, &highPriorityTaskWoken);
        }
	}
}

//------------------------------------------------------------------------------
void TIM2_IRQHandler(void) 
{
    HAL_TIM_IRQHandler(&timer2Handle);
    
/*    if (__HAL_TIM_GET_FLAG(&timer2Handle, TIM_FLAG_UPDATE) != RESET) { // In case other interrupts are also running
        if (__HAL_TIM_GET_ITSTATUS(&timer2Handle, TIM_IT_UPDATE) != RESET)  {
            __HAL_TIM_CLEAR_FLAG(&timer2Handle, TIM_FLAG_UPDATE);
            if (commQueue1 != NULL) {
            	xQueueSendToBackFromISR(commQueue1, &cIn, &highPriorityTaskWoken);
            }
        }
	}*/
}


void SPI1_IRQHandler(void)
{
    HAL_SPI_IRQHandler(&spi1Handle);
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&uart1Handle);
}
