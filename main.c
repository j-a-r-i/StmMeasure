#include "hw.h"
#include "hal.h"
//#include "mysensor.h"
#include "outsens.h"
#include "sump.h"
#include "ds1820.h"
#include "rfm12b.h"

#define NULL (void*)0

char gVersion[] = "V0.0.3\r\n\000";

void     SystemClock_Config(void);

#define DELAY 100
#define UART  1
uint32_t gEvents;
uint8_t gState;
uint8_t gCounter;

#define TEMP_COUNT 3
uint16_t temp[TEMP_COUNT];
rfm12b rfm1;
rfm12b rfm2;

void cmdHelp();
void cmdVersion();
void cmdTest1();
void cmdTest2();
void state_change();

typedef struct MenuItem {
    char key;
    char *description;
    void (*func)();
} menuitem_t;

menuitem_t gMainMenu[] = {
    {'h', "help", cmdHelp},
    {'s', "state change", state_change},
    {'v', "version", cmdVersion},
    {'1', "test 1", cmdTest1},
    {'2', "test 2", cmdTest2},
    { 0,  NULL, NULL}
};



//------------------------------------------------------------------------------
void state_change()
{
    #define BUF_SIZE 5
    uint8_t buffer[] = { 'h', 'e', 'l', 'l', 'o' };

    rfm12b_send(&rfm1, buffer, sizeof(buffer));
    
    
    switch (gState) {
    case 0:
	ds1820_measure(PIN_DS1820a);
	//set_LED1;
	break;
    case 2:
	temp[0] = ds1820_read_temp(PIN_DS1820a);
	//clr_LED1;
	break;
    case 3:
	//uart_sends(mysensor_set(1,1, V_TEMP, temp[0]));
	uart_sends(UART, outsens_set(gCounter, temp, TEMP_COUNT));
	gCounter++;
	break;
    }
    gState++;
    if (gState > 59)
	gState = 0;
}


void cmdVersion()
{
    uart_sends(UART, gVersion);
    uart_send_nl(UART);
}

void cmdHelp()
{
    uint8_t i;

    for (i=0; gMainMenu[i].key != 0; i++) {
	uart_send(UART, gMainMenu[i].key);
	uart_send(UART, ' ');	
	uart_sends(UART, gMainMenu[i].description);
	uart_send_nl(UART);
    }
}

void cmdTest1()
{
    char buffer[]="hello";
    
    rfm12b_send(&rfm1, (uint8_t*)buffer, 5);
}

void cmdTest2()
{
    rfm12b_test(&rfm1);
}

//------------------------------------------------------------------------------
void command(uint8_t cmd)
{
    uint8_t i;
    uint8_t found = 0;
    tgl_LED1;

    for (i=0; gMainMenu[i].key != 0; i++) {
	if (gMainMenu[i].key == cmd) {
	    (*(gMainMenu[i].func))();
	    found = 1;
	    break;
	}
    }

    if (!found) {
	uart_sends(UART, "invalid commmand!\r\n");
    }
}

//------------------------------------------------------------------------------
int main()
{
    gEvents = 0;
    gState = 0;
    
    SystemClock_Config();

    io_init();
    uart_init(UART);
    timer2_init();

    ds1820_init(PIN_DS1820a);
    rfm12b_init(&rfm1, 1, PIN_RFM12_SEL1, PIN_RFM12_IRQ1);
    rfm12b_init(&rfm2, 2, PIN_RFM12_SEL2, PIN_RFM12_IRQ2);
    
    outsens_init();
    //mysensor_init();

    temp[1] = 2;
    temp[2] = 3;
    
    uart_sends(UART, "starting..\r\n");
    //uart_sends(mysensor_present(1,1, S_TEMP));

/*    while (1) {
	uint16_t i;

	io_set(PIN_LED1);
	for (i=0; i<1000; i++) {
	    delay_us(1000);
	}

	io_clear(PIN_LED1);
	for (i=0; i<1000; i++) {
	    delay_us(1000);
	}
    }*/
    /*while (1) {
	rfm12b_test(&rfm1);
	delay_us(00);
    }*/
    /*while (1) {
	io_set(PIN_RFM12_SEL1);
	io_set(PIN_LED1);
	delay_us(2000);
	io_clear(PIN_RFM12_SEL1);
	io_clear(PIN_LED1);
	delay_us(2000);
    }*/
    
    while (1) {
	if (gEvents & EV_TIMER2) {
	    tgl_LED2;
	    state_change();

	    gEvents &= ~(EV_TIMER2);
	}
	if (gEvents & EV_UART1_RX) {
	    //LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_8);

	    //sump_handle(gUartRx1);
	    command(gUart1Rx);
	    
	    gEvents &= ~(EV_UART1_RX);
	}
	if (gEvents & EV_UART2_RX) {
	    command(gUart2Rx);
	    
	    gEvents &= ~(EV_UART2_RX);
	}
	if (gEvents & EV_SPI1_RX) {
	    //command(gSpi1Rx);
	    
	    gEvents &= ~(EV_SPI1_RX);
	}
	if (gEvents & EV_SPI2_RX) {
	    //command(gSpi2Rx);
	    
	    gEvents &= ~(EV_SPI2_RX);
	}
    }
}

//------------------------------------------------------------------------------
void error(error_t code)
{
    while (1) {
	set_LED1;
	set_LED2;
	LL_mDelay(200);

	clr_LED1;
	clr_LED2;
	LL_mDelay(200);
    }
}

//------------------------------------------------------------------------------
void SystemClock_Config(void)
{
#ifdef stm32f0
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1);

    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,
				LL_RCC_PLL_MUL_12);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

    LL_Init1msTick(48000000);

    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

    LL_SetSystemCoreClock(48000000);

    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, 3);
    
#else
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
#endif
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    uart_sends("ASSERT ERROR in ");
    uart_sends(file);
    uart_send_nl();

    while (1);
}
#endif
