//**************************** UartEventHandler *******************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartEventHandler.h
// Summary  : UART RX buffer, event handlers, and frame extraction for nRF52
// Author   : Anoop
// Date     : 20-07-2025
//*****************************************************************************
#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

//******************************* Include Files ******************************
#include <stdint.h>
#include "common.h"
#include "UartApi.h"

//***************************** Global Constants ******************************
#define UART_RX_PIN_NUMBER    8
#define UART_TX_PIN_NUMBER    6
#define UART_RTS_PIN_NUMBER   5
#define UART_CTS_PIN_NUMBER   7
#define UART_BAUDRATE         UART_BAUDRATE_BAUDRATE_Baud115200

//**************************** Forward Declarations ***************************
bool UartInit(void);
void UartHandleError(UART_EVENT *psEvent);
void UartEventHandle(UART_EVENT *psEvent);

#endif // FILE_TRANSFER_H
// EOF
