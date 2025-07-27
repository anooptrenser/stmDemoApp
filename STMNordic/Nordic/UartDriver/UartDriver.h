//******************************* UartDriver **********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File    : UartDriver.h
// Summary : UART abstraction header for Nordic app_uart backend
// Author  : <Your Name>
// Date    : 17-07-2025
//
//*****************************************************************************

#ifndef UART_DRIVER_H
#define UART_DRIVER_H

//********************* Include Files *****************************************
#include "common.h"

//******************************* Global Types ******************************** 

//***************************** Global Variables ****************************** 

//*************************** Global Constants ********************************

//************************* Function Declarations *****************************
bool UartSend(const uint8* pucData, uint32 ulLen);

#endif // UART_DRIVER_H

// EOF
