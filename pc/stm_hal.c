#include "hw.h"

void LL_USART_TransmitData8(USART_TypeDef *USARTx, uint8_t Value)
{
    USARTx->TDR = Value;
}

uint8_t LL_USART_ReceiveData8(USART_TypeDef *USARTx)
{
    return USARTx->RDR;
}

uint32_t LL_USART_IsActiveFlag_TXE(USART_TypeDef *USARTx)
{
    return USARTx->TXE;
}

uint32_t LL_USART_IsActiveFlag_RXNE(USART_TypeDef *USARTx)
{
    return USARTx->RXNE;
}

uint32_t LL_USART_IsActiveFlag_PE(USART_TypeDef *USARTx)
{
    return 0;
}

uint32_t LL_USART_IsActiveFlag_FE(USART_TypeDef *USARTx)
{
    return 0;
}

uint32_t LL_USART_IsActiveFlag_NE(USART_TypeDef *USARTx)
{
    return 0;
}

uint32_t LL_USART_IsActiveFlag_ORE(USART_TypeDef *USARTx)
{
    return 0;
}

void LL_USART_ClearFlag_PE(USART_TypeDef *USARTx)
{
}

void LL_USART_ClearFlag_FE(USART_TypeDef *USARTx)
{
}

void LL_USART_ClearFlag_NE(USART_TypeDef *USARTx)
{
}

void LL_USART_ClearFlag_ORE(USART_TypeDef *USARTx)
{
}

void LL_USART_EnableIT_TXE(USART_TypeDef *USARTx)
{
    USARTx->INT_TXE = 1;
}
void LL_USART_DisableIT_TXE(USART_TypeDef *USARTx)
{
    USARTx->INT_TXE = 0;
}

void LL_USART_EnableIT_RXNE(USART_TypeDef *USARTx)
{
}

void LL_USART_EnableIT_ERROR(USART_TypeDef *USARTx)
{
}

void LL_USART_Enable(USART_TypeDef *USARTx)
{
}

void LL_USART_ConfigAsyncMode(USART_TypeDef *USARTx)
{
}

void LL_USART_SetBaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t OverSampling,
                                          uint32_t BaudRate)
{
}

void LL_USART_ConfigCharacter(USART_TypeDef *USARTx, uint32_t DataWidth, uint32_t Parity,
                                              uint32_t StopBits)
{
}

void LL_USART_SetTransferDirection(USART_TypeDef *USARTx, uint32_t TransferDirection)
{
}

uint32_t LL_USART_IsEnabledIT_RXNE(USART_TypeDef *USARTx)
{
    return 1;
}

uint32_t LL_USART_IsEnabledIT_TXE(USART_TypeDef *USARTx)
{
    return USARTx->INT_TXE;
}
