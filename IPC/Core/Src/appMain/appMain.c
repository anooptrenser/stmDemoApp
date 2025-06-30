//**************************** AppMain *****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Application main entry point implementation.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#include "appMain.h"
#include "osQueue.h"
#include "pollerTasks.h"
#include "receiverTasks.h"
#include <stdio.h>
#include <stdbool.h>

//******************************.AppMain.**************************************
// Purpose : Main entry point for application. Initializes OS components and creates tasks.
// Inputs  : None
// Outputs : None
// Notes   : Handles task creation failures as per Trenser Embedded Coding Standard.
//**********************************************************************************
void AppMain(void)
{
    // Initialize OS components
    OsQueueInit();

    // Create tasks and check for failures
    if (!PollerTaskCreate())
    {
        printf("Error: PollerTask creation failed\r\n");
        return;
    }

    if (!ReceiverTaskCreate())
    {
        printf("Error: ReceiverTask creation failed\r\n");
        return;
    }
}

//EOF
