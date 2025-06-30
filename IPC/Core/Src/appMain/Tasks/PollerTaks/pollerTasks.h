//**************************** PollerTasks *************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for Poller Task and related constants.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef POLLER_TASK_H
#define POLLER_TASK_H

//******************************* Include Files ********************************
#include "cmsis_os2.h"
#include <stdbool.h>

//******************************* Constants ************************************
#define POLLER_TASK_STACK_SIZE    (128 * 8)
#define POLLER_TASK_DELAY_MS      (10)

//******************************* Function Prototypes **************************
bool PollerTaskCreate(void);
void PollerTaskRun(void *argument);

#endif /* POLLER_TASK_H */
