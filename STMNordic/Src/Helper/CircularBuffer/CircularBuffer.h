//******************************* FileTransferHelper *************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : CircularBuffer.h
// Summary  : Implements a simple circular buffer for UART RX data handling.
// Author   : Anoop G
// Date     : 20-07-2025
//
//*****************************************************************************
#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

//******************************* Include Files *******************************
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

//***************************** Global Constants ******************************
#define UART_RX_BUF_SIZE     512

//******************************* Global Types ********************************
typedef struct
{
    uint8 pucBuffer[UART_RX_BUF_SIZE];
    volatile uint16 unHead;
    volatile uint16 unTail;
} UART_RX_BUFFER;

//**************************** Function Declarations ***************************
bool UartRxBufferInit(UART_RX_BUFFER* pRxBuffer);
bool UartRxBufferPush(UART_RX_BUFFER* pRxBuffer, uint8 ucData);
bool UartRxBufferPop(UART_RX_BUFFER* pRxBuffer, uint8* pucData);
uint16 UartRxBufferCount(UART_RX_BUFFER* pRxBuffer);

#endif // _CIRCULAR_BUFFER_H_

// EOF
