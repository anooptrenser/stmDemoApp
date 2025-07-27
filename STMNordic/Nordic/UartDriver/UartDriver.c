//******************************* UartDriver **********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File    : UartDriver.c
// Summary : UART driver implementation for Nordic using app_uart
// Author  : Anoop G
// Date    : 17-07-2025
//
//*****************************************************************************

//********************* Include Files *****************************************
#include "UartDriver.h"
#include "app_uart.h"
#include "app_timer.h"
#include <stdio.h>   

//******************************* Local Types ********************************* 

//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 

//******************************.FUNCTION_HEADER.******************************
// Function : UartSend
// Purpose  : Sends bytes over UART using app_uart
// Inputs   : pucData - pointer to buffer
//            ulLen   - number of bytes to send
// Returns  : bool - TRUE on success, FALSE on timeout or error
//*****************************************************************************
bool UartSend(const uint8* pucData, uint32 ulLen)
{
    bool blSuccess = false;
    uint32 ulIndex = 0U;

    if ((pucData == NULL) || (ulLen == 0U))
    {
        blSuccess = false;
    }
    else
    {
        for (ulIndex = 0U; ulIndex < ulLen; ++ulIndex)
        {
            while (app_uart_put(pucData[ulIndex]) != NRF_SUCCESS)
            {
                /* Retry until transmitter is ready */
            }
        }

        blSuccess = true;
    }

    return blSuccess;
}


// EOF
