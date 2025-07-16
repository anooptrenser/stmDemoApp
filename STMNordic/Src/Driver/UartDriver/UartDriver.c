//*******************************UartDriver************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : UartDriver.c
//Summary  : UART abstraction layer implementation
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <common.h>
#include "UartDriver.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

//******************************.FUNCTION_HEADER.******************************
//Purpose : Send data over UART
//Inputs  : pucData - pointer to data buffer
//          ulLen   - number of bytes to send
//Outputs : None
//Return  : TRUE if successful, FALSE otherwise
//Notes   : None
//*****************************************************************************
bool UartSend(const uint8* pucData, uint32 ulLen)
{
    return (HAL_UART_Transmit(&huart2, (uint8*)pucData, ulLen, TIMEOUT) == HAL_OK);
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : Receive data from UART
//Inputs  : pucData - pointer to data buffer
//          ulLen   - number of bytes to receive
//          ulTimeoutMs - timeout in ms
//Outputs : None
//Return  : TRUE if successful, FALSE otherwise
//Notes   : None
//*****************************************************************************
bool UartReceive(uint8* pucData, uint32 ulLen, uint32 ulTimeoutMs)
{
    return (HAL_UART_Receive(&huart2, pucData, ulLen, ulTimeoutMs) == HAL_OK);
}

//EOF
