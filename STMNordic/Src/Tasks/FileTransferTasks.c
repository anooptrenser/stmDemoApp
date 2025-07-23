//******************************* FileTransferTasks ***************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferTasks.c
// Summary  : RTOS task for handling file transfers
// Author   : Anoop G
// Date     : 22-07-2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include <stdio.h>
#include <stdint.h>
#include "FileTransferTasks.h"
#include "FileTransferManager.h"
#include "Tmp.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************

//******************************.FUNCTION_HEADER.******************************
// Purpose : Thread to handle file transfer via custom protocol
// Inputs  : pvArgs - Not used
// Outputs : None
// Return  : None
// Notes   : Runs once or can be extended to run in a loop or task queue
//******************************************************************************
void FileTransferManagerTask(void *pvArgs)
{
    (void)pvArgs;  // Suppress unused warning

    if (!FileTransferManager(g_ucData, g_ulDataLen))
    {
        printf("Error: Data transfer failed!\r\n");
    }
}
