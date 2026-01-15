//**************************** ReceiverTasks **********************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for Receiver Task and related constants.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef RECEIVERTASK_H
#define RECEIVERTASK_H

//******************************* Include Files *******************************
#include "cmsis_os2.h"
#include "common.h"
#include <stdbool.h>
#include "osQueue.h"

//******************************* Constants ***********************************
#define RECEIVER_TASK_STACK_SIZE    (128 * 8)
#define RECEIVER_TASK_PRIORITY      (osPriorityNormal)

//******************************* Global Types ********************************
typedef void (*CMD_HANDLER)(const REQUEST_MESSAGE*, ACK_MESSAGE*);

//******************************* Function Prototypes *************************
bool ReceiverTaskCreate(void);
void ReceiverTaskRun(void* pArgument);


#endif /* RECEIVERTASK_H */

//EOF
