//*******************************UartDriver************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : UartDriver.h
//Summary  : UART abstraction layer header
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

#ifndef UARTDRIVER_H
#define UARTDRIVER_H

//*************************** Include Files************************************
#include <common.h>
#include <stdbool.h>
#include "main.h"
#include "CircularBuffer.h"

//*************************** Global Constants ********************************
#define TIMEOUT 3000

//*************************** Global Variables*********************************
extern uint8 ucData;
extern UART_RX_BUFFER gUartRxBuffer;
extern UART_HandleTypeDef huart2;
extern void *gFrameQueueHandle;

//*********************Forward Declarations************************************
bool UartSend(const uint8* pucData, uint32 ulLen);
bool UartIntrInit(void *pvUartHandler, uint8 *pucBuffer, uint16 unBufferSize);
bool InitUartFrameQueue(void);

#endif // UARTDRIVER_H

//EOF
