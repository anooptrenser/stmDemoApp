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

//*********************Include Files*******************************************
#include <common.h>
#include <stdbool.h>

//***************************** Global Constants *******************************
#define TIMEOUT 1000

//*********************Forward Declarations************************************
void UartInit(void);
bool UartSend(const uint8* pucData, uint32 ulLen);
bool UartReceive(uint8* pucData, uint32 ulLen, uint32 ulTimeoutMs);

#endif // UARTDRIVER_H

//EOF
