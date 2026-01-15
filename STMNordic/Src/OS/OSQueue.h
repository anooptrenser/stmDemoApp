//******************************.HEADER.***************************************
// File     : OSQueue.h
// Summary  : RTOS queue wrapper interface
// Author   :
// Date     : 21/07/2025
//*****************************************************************************
#ifndef __OSQUEUE_H__
#define __OSQUEUE_H__

//******************************* Include Files *******************************
#include <stdint.h>
#include <stdbool.h>
#include "common.h"

//************************** Function Declarations ****************************
bool OSQueueCreate(void **ppvQueueHandle, uint32 ulQElmntSize, uint32 ulQElmntCnt);
bool OSQueueSend(void *pvQueueHandle, void *pvBuffer, uint32 ulWaitTicks);
bool OSQueueRecv(void *pvQueueHandle, void *pvBuffer, uint32 ulWaitTicks);

#endif // __OSQUEUE_H__

//EOF
