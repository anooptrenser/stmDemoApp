//**************************** AppMain ****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Application main entry point implementation.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include <stdio.h>
#include <stdbool.h>
#include "appMain.h"
#include "osQueue.h"

//******************************.AppMain.**************************************
// Purpose : Main entry point for application.
// Inputs  : None
// Outputs : None
// Notes   : None
//*****************************************************************************
void AppMain(void)
{
    for (int8 i = 0; i < TASK_COUNT; ++i)
    {
        if (!taskCreateTable[i]())
        {
            printf("Error: Task %d creation failed\r\n", i);
            return;
        }
    }
}

//EOF
