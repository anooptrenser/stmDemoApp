//**************************** PollerTasks ************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for Poller Task and related constants.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
#ifndef POLLERTASK_H
#define POLLERTASK_H

//******************************* Include Files *******************************
#include <stdbool.h>
#include "cmsis_os2.h"
#include "common.h"

//******************************* Constants ***********************************
#define POLLER_TASK_STACK_SIZE    (128 * 8)
#define POLLER_TASK_DELAY_MS      (10)
#define POLLER_TASK_PRIORITY      (osPriorityHigh)
#define LED_ON                    1

//******************************* Global Variables ****************************
extern const osThreadAttr_t PollerTaskAttributes;

//******************************* Function Prototypes *************************
bool PollerTaskCreate(void);
void PollerTaskRun(void* pArgument);
bool IsButtonStateValid(uint32 ulButtonState);
bool IsButtonPressed(uint32 ulLastState, uint32 ulCurrentState);
void PollerButtonPress(uint32* pulTransactionUid, uint8* pucLedOn, uint8* pucToggle);
void ProcessAckMessages(void);

#endif /* POLLERTASK_H */

//EOF
