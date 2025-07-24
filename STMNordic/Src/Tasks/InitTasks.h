//******************************* InitTasks.h *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Tasks.h
// Summary  : Header for the RTOS task initialization function
// Author   : Anoop G
// Date     : 22-07-2025
//
//*****************************************************************************
#ifndef INIT_TASKS_H
#define INIT_TASKS_H 
//******************************* Include Files *******************************
#include <stdbool.h>
#include "OSFactory.h"

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
typedef enum __eTaskId
{
    UART_FRAME_RECEIVER_TASK,
    FILE_TRANSFER_MANAGER_TASK,
} _eTaskId;

//***************************** Global Structures *****************************
typedef struct __sTaskTable
{
    _eTaskId                eTaskId;
    _sOsFactThreadAttributes *psTaskAttr;
    void                    *pvTaskRoutine;
    void                    *pvArgs;
}_sTaskTable;

//**************************** Forward Declarations ***************************
bool InitTasks(void);

#endif // INIT_TASKS_H

//EOF
