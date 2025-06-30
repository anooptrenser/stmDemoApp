//**************************** ReceiverTask ***********************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for Receiver Task and related constants.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef RECEIVER_TASK_H
#define RECEIVER_TASK_H

//******************************* Include Files *******************************
#include "cmsis_os2.h"
#include <stdbool.h>

#define RECEIVER_TASK_STACK_SIZE    (128 * 8)
#define RECEIVER_TASK_PRIORITY      (osPriorityNormal)

bool ReceiverTaskCreate(void);
void ReceiverTaskRun(void *argument);

#endif /* RECEIVER_TASK_H */

//EOF
