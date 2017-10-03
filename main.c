#include "hw.h"
#include "hal.h"
#include "mysensor.h"
#include "sump.h"

void     SystemClock_Config(void);

#define DELAY 100

uint32_t gEvents;
uint8_t gState;
uint8_t temp;

//------------------------------------------------------------------------------
void state_change()
{
    switch (gState) {
    case 0:
	set_LED1;
	clr_LED2;
	break;
    case 1:
	break;
    case 2:
	clr_LED1;
	set_LED2;
	break;
    case 3:
	//uart_sends(mysensor_set(1,1, V_TEMP, temp));
	temp++;
	break;
    default:
	break;
    }

    gState++;
    if (gState == 4)
	gState = 0;
}
//------------------------------------------------------------------------------
int main()
{
    temp = 1;
    gEvents = 0;
    gState = 0;
    
    SystemClock_Config();

    io_init();
    uart_init();
    timer2_init();

    //uart_sends(mysensor_present(1,1, S_TEMP));
	           
    while (1) {
	if (gEvents & EV_TIMER2) {
	    state_change();

	    gEvents &= ~(EV_TIMER2);
	}
	if (gEvents & EV_UART_RX) {
	    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_14);

	    sump_handle(gUartRx);
	    
	    gEvents &= ~(EV_UART_RX);
	}
    }
}

//------------------------------------------------------------------------------
void error(uint8_t code)
{
    while (1) {
	set_LED1;
	set_LED2;
	set_LED3;
	LL_mDelay(200);

	clr_LED1;
	clr_LED2;
	clr_LED3;
	LL_mDelay(200);
    }
}

//------------------------------------------------------------------------------
void SystemClock_Config(void)
{
    LL_RCC_HSE_EnableBypass();
    LL_RCC_HSE_Enable();
    while (LL_RCC_HSE_IsReady() != 1);

    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE,
				LL_RCC_PLLM_DIV_8,
				400,
				LL_RCC_PLLP_DIV_4);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    SysTick_Config(100000000 / 1000);  // 1ms

    //SystemCoreClock = 168000000;
    SystemCoreClock = 100000000;
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    uart_sends("ASSERT ERROR in ");
    uart_sends(file);
    uart_sends("!\r\n");

    while (1);
}
#endif
