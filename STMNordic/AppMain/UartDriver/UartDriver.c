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
#include "UartDriver.h"
#include "main.h" //
#include "common.h"

extern UART_HandleTypeDef huart2;

//*********************.UartInit.**********************************************
//Purpose : UART initialization (already done in main.c)
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void UartInit(void)
{
    // UART2 already initialized in main.c
}

//*********************.UartSend.**********************************************
//Purpose : Send data over UART
//Inputs  : pucData - pointer to data buffer
//          ulLen   - number of bytes to send
//Outputs : None
//Return  : TRUE if successful, FALSE otherwise
//Notes   : None
//*****************************************************************************
bool UartSend(const uint8* pucData, uint32 ulLen)
{
    return (HAL_UART_Transmit(&huart2, (uint8*)pucData, ulLen, 1000) == HAL_OK);
}

//*********************.UartReceive.*******************************************
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
