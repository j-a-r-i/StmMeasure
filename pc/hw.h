#pragma once

#define UART 2

#include <stdio.h>
#include <stdint.h>

#include "hal.h"
#include "buffer.h"

/** Error codes
 */
typedef enum errors {
    ERR_HARD_FAULT         = 0x01,  // CPU fault

    ERR_UART_PARITY,        // 02
    ERR_UART_FRAMING,       // 03
    ERR_UART_NOISE,         // 04
    ERR_UART_OVERRUN,       // 05

    ERR_INVALID_PIN,        // 06
    ERR_UART_PORT,          // 07
    ERR_SPI_PORT,           // 08
    ERR_INVALID_MEAS,       // 09
    ERR_INVALID_SCHE,       // 0A
    ERR_BUFFER_OVERFLOW,    // 0B
    ERR_ARG_COUNT,          // 0C
    ERR_INVALID_COMMAND,    // 0D
    ERR_SYNTAX,             // 0E
    ERR_ARGUMENT            // 0F
} error_t;

/** Function pointer for multiline printing,
 *  when there is more printing that buffer_t
 *  can hold.
 */
typedef uint8_t (*func_mline)(uint8_t timer, buffer_t *buf);

extern func_mline gFuncMLine;


extern void _error(error_t code);
extern void trace(uint8_t ch);


// simulated stm32 hal layer 
//
typedef struct {
    uint8_t TXE;
    uint8_t RXNE;

    uint8_t INT_TXE;

    uint8_t TDR;
    uint8_t RDR;
} USART_TypeDef;

extern USART_TypeDef *USART1;
extern USART_TypeDef *USART2;

#define LL_USART_DIRECTION_TX_RX 0xFFFF
#define LL_USART_DATAWIDTH_8B 0xFFFF
#define LL_USART_PARITY_NONE 0xFFFF
#define LL_USART_STOPBITS_1 0xFFFF
#define LL_USART_OVERSAMPLING_16 0xFFFF
#define SystemCoreClock 8000000

extern void LL_USART_TransmitData8(USART_TypeDef *USARTx, uint8_t Value);
extern uint8_t LL_USART_ReceiveData8(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_TXE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_RXNE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_PE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_FE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_NE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsActiveFlag_ORE(USART_TypeDef *USARTx);
extern void LL_USART_ClearFlag_PE(USART_TypeDef *USARTx);
extern void LL_USART_ClearFlag_FE(USART_TypeDef *USARTx);
extern void LL_USART_ClearFlag_NE(USART_TypeDef *USARTx);
extern void LL_USART_ClearFlag_ORE(USART_TypeDef *USARTx);
extern void LL_USART_EnableIT_TXE(USART_TypeDef *USARTx);
extern void LL_USART_DisableIT_TXE(USART_TypeDef *USARTx);
extern void LL_USART_EnableIT_RXNE(USART_TypeDef *USARTx);
extern void LL_USART_EnableIT_ERROR(USART_TypeDef *USARTx);
extern void LL_USART_Enable(USART_TypeDef *USARTx);
extern void LL_USART_ConfigAsyncMode(USART_TypeDef *USARTx);
extern void LL_USART_SetBaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t OverSampling,
				 uint32_t BaudRate);
extern void LL_USART_ConfigCharacter(USART_TypeDef *USARTx, uint32_t DataWidth, uint32_t Parity,
				     uint32_t StopBits);
extern void LL_USART_SetTransferDirection(USART_TypeDef *USARTx, uint32_t TransferDirection);
extern uint32_t LL_USART_IsEnabledIT_RXNE(USART_TypeDef *USARTx);
extern uint32_t LL_USART_IsEnabledIT_TXE(USART_TypeDef *USARTx);
