//*******************************AppMain***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : AppMain.c
//Summary  : Application entry point
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include <stdio.h>
#include "AppMain.h"
#include "FileTransferManager.h"
#include "Tmp.h"
#include "UartDriver.h"
#include "UartFrameReceiverTasks.h"
#include "CircularBuffer.h"
#include <ctype.h>
#include <stdint.h>
#include "cmsis_os2.h"
#include "main.h"
#include "OsFactory.h"
#include "UartDriverInit.h"
#include "InitTasks.h"

//***************************** Global Variables ******************************

//******************************.FUNCTION_HEADER.******************************
// Purpose : Application entry point for initializing peripherals and tasks
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Initializes UART peripheral, queue system and starts FreeRTOS tasks.
//*****************************************************************************
void AppMain(void)
{
    // Initialize the UART subsystem (buffer, queue, interrupt)    
    if (!UartSubsystemInit())
    {
        printf("[ERROR] UART subsystem failed to initialize\n");
    }

    // Create all tasks using the function table
    if (!InitTask())
    {
        printf("Task initialization failed!\r\n");
    }

    OsTaskDelay(1000);
}

//EOF
