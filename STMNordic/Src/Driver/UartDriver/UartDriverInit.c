//**************************** PROJECT X ****************************** 
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
    bool blSuccess = true;

    // Initialize circular buffer
    if (!UartRxBufferInit(&gUartRxBuffer))
    {
        printf("[UART INIT] Failed to init RX ring buffer\n");
        blSuccess = false;
    }

    // Init frame queue for higher-level protocol dispatch (if used)
    if (!InitUartFrameQueue())
    {
        printf("[UART INIT] Failed to init UART frame queue\n");
        blSuccess = false;
    }

    // Start single-byte UART RX interrupt-based reception
    if (!UartIntrInit(&huart2, &ucData, 1))
    {
        printf("[UART INIT] Failed to start UART RX interrupt\n");
        blSuccess = false;
    }

    return blSuccess;
}