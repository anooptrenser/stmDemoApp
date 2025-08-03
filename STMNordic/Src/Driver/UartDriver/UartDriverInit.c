//**************************** UartDriverInit *********************************
// Copyright (c) 2021 Trenser 
// All Rights Reserved 
//***************************************************************************** 
//
// File     : UartDriverInit.c
// Summary  : UART driver subsystem initialization
// Note     : Initializes UART RX buffer, frame queue, and interrupt
// Author   : Anoop G
// Date     : 14-07-2025
//
//***************************************************************************** 
//******************************* Include Files *******************************
#include "UartDriverInit.h"
#include "CircularBuffer.h"
#include "OsFactory.h"
#include "UartDriver.h"

//***************************** Local Variables *******************************

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initializes the UART subsystem (buffer, queue, interrupt)
// Inputs  : None
// Outputs : None
// Return  : bool - TRUE if all UART components initialized successfully, 
//                  FALSE otherwise
// Notes   : Call this during system startup before using UART
//*****************************************************************************
bool UartSubsystemInit(void)
{
    bool blSuccess = false;

    if (UartRxBufferInit(&gUartRxBuffer))
    {
        if (UartFrameQueueInit())
        {
            if (UartIntrInit(&huart2, &ucData, 1))
            {
                blSuccess = true;
            }
            else
            {
                printf("[UART INIT] Failed to start UART RX interrupt\n");
            }
        }
        else
        {
            printf("[UART INIT] Failed to init UART frame queue\n");
        }
    }
    else
    {
        printf("[UART INIT] Failed to init RX ring buffer\n");
    }

    return blSuccess;
}

//EOF