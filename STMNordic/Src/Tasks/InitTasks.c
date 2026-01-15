//******************************* Tasks.c *************************************
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

//******************************* Local Types *********************************
typedef struct
{
    uint8       *pucTaskName;    // Task Name
    osThreadId_t Thread_Id;      // Thread ID
    uint32       ulStackSize;    // Stack Size
    uint32       ulPriority;     // Task Priority
    void       (*pTaskFunc)(void *); // Thread Entry Function
} TASKS;

//***************************** Local Constants *******************************
#define TASKS_MAX_SIZE   2
#define STACK_SIZE       5120
#define THREAD_PRIORITY_NORMAL  osPriorityNormal
#define THREAD_PRIORITY_HIGH    osPriorityHigh

//***************************** Local Variables *******************************
static TASKS sTasks[TASKS_MAX_SIZE] =
{
    { (uint8 *)"UartReceiver",     0, STACK_SIZE, THREAD_PRIORITY_HIGH, UartFrameReceiverTask },
    { (uint8 *)"FileTransferMgr",  0, STACK_SIZE, THREAD_PRIORITY_NORMAL, FileTransferManagerTask }
};

//****************************** Local Functions ******************************

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initializes and starts all RTOS tasks defined in the task table
// Inputs  : None
// Outputs : None
// Return  : bool - TRUE if all tasks created successfully, FALSE otherwise
// Notes   : Called during system startup to launch application threads
//******************************************************************************
bool InitTask(void)
{
    bool blAllCreated = false;

    for (uint8 ucIdx = 0; ucIdx < TASKS_MAX_SIZE; ucIdx++)
    {
        osThreadAttr_t stAttr = {0};
        stAttr.name       = (const char *)sTasks[ucIdx].pucTaskName;
        stAttr.stack_size = sTasks[ucIdx].ulStackSize;
        stAttr.priority   = (osPriority_t)sTasks[ucIdx].ulPriority;

        sTasks[ucIdx].Thread_Id = osThreadNew(
                                        sTasks[ucIdx].pTaskFunc,
                                        NULL,
                                        &stAttr);

        if (sTasks[ucIdx].Thread_Id == NULL)
        {
            printf("Error: Failed to create task: %s\r\n", sTasks[ucIdx].pucTaskName);
            blAllCreated = false;
        }
        else
        {
            printf("Task created: %s\r\n", sTasks[ucIdx].pucTaskName);
            blAllCreated = true;
        }
    }

    return blAllCreated;
}

//EOF