//**************************** AppMain ****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for application main entry point.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
#ifndef APP_MAIN_H
#define APP_MAIN_H

//******************************* Include Files *******************************
#include "stm32f4xx_hal.h"
#include "pollerTasks.h"
#include "receiverTasks.h"

//****************************** Constants ************************************
#define TASK_COUNT (sizeof(taskCreateTable)/sizeof(taskCreateTable[0]))

//******************************* Global Types ********************************
typedef bool (*TaskCreateFunc)(void);

static const TaskCreateFunc taskCreateTable[] = {
    PollerTaskCreate,
    ReceiverTaskCreate
};

//******************************* Function Prototypes **************************
void AppMain(void);

#endif /* APP_MAIN_H */

//EOF
