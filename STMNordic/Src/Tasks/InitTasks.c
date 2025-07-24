//******************************* InitTasks.c *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Tasks.c
// Summary  : Centralized RTOS task registry and startup logic
// Author   : Anoop G
// Date     : 22-07-2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os2.h"
#include "InitTasks.h"
#include "common.h"
#include "UartFrameReceiverTasks.h"
#include "FileTransferTasks.h"

//***************************** Local Variables *******************************
static _sOsFactThreadAttributes sucUartFrameReceiverTaskAttr = 
    {UART_FRAME_RECEIVER_TASK_NAME, MEDIUM, UART_FRAME_RECEIVER_TASK_STACK_SIZE};
static _sOsFactThreadAttributes sucFileTransferMngrTaskAttr = 
    {FILE_TRANSFER_MNGR_TASK_NAME, LOW, FILE_TRANSFER_MNGR_TASK_STACK_SIZE};

static _sTaskTable sTaskTable[] = {
    {UART_FRAME_RECEIVER_TASK, &sucUartFrameReceiverTaskAttr, UartFrameReceiverTask, NULL},
    {FILE_TRANSFER_MANAGER_TASK, &sucFileTransferMngrTaskAttr, FileTransferManagerTask, NULL}
};

//****************************** Local Functions ******************************

//*************************** Function Definition *****************************

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initializes and starts all RTOS tasks defined in the task table
// Inputs  : None
// Outputs : None
// Return  : bool - TRUE if all tasks created successfully, FALSE otherwise
// Notes   : Called during system startup to launch application threads
//******************************************************************************
bool InitTasks(void)
{
    bool bRetValue = false;
    uint8_t ucIndex = 0;
    uint32_t ulTaskTableLength = 0;

    ulTaskTableLength = sizeof(sTaskTable) / sizeof(_sTaskTable);

    for (ucIndex = 0; ucIndex < ulTaskTableLength; ucIndex++)
    {
        if (!OsFactCreateThread(sTaskTable[ucIndex].psTaskAttr,
                                sTaskTable[ucIndex].pvTaskRoutine,
                                sTaskTable[ucIndex].pvArgs))
        {
            break;  
        }
        else
        {
            bRetValue = true;  
        }
    }

    return bRetValue;
}

//EOF