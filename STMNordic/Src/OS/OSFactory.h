//**************************** OS_FACTORY *******************************
// Copyright (c) 2025
// All Rights Reserved
//*****************************************************************************
//
// File     : OsFactory.h
// Summary  : File containing OS functions
// Note     : None
// Author   : None
// Date     : 12/06/2025
//
//*****************************************************************************

#ifndef __OS_FACTORY_H__
#define __OS_FACTORY_H__

//******************************* Include Files *******************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include "FreeRTOSConfig.h"

//********************************** MACROS ***********************************
#define OS_MAX_DELAY    portMAX_DELAY

//***************************** Global Constants ******************************
typedef enum __eThreadPriority
{
    HIGH        = 5,
    MEDIUM      = 4,
    LOW         = 3
}_eThreadPriority;

//***************************** Global Structures *****************************
typedef struct __sOsFactThreadAttributes
{
    const char *pcThreadName;
    _eThreadPriority eThreadPriority;
    int nThreadStackSize;
}_sOsFactThreadAttributes;

//***************************** Function Declaration **************************
bool OsCreateEventGroup(EventGroupHandle_t* pOsEventsHandle);
bool OsSetEventBitsFromISR(EventGroupHandle_t* pOsEventsHandle, uint32_t ulSetEventBits);
uint32_t OsGetEventBits(EventGroupHandle_t* pOsEventsHandle);
bool OsClearEventBits(EventGroupHandle_t* pOsEventsHandle, uint32_t ulClearEventBits);
bool OsSetEventBits(EventGroupHandle_t* pOsEventsHandle, uint32_t ulSetEventBits);
uint32_t  OsWaitEvent(EventGroupHandle_t* pOsEventsHandle, uint32_t ulWaitTimeout, uint32_t ulEvents, bool bClearOnexit);
bool OsSoftTimerCreate(void** pTimerHandle, uint8_t* pucTimerName, uint32_t ulTimePeriod,
							bool bAutoReload, void* pTimerId, void* pCallBack);
// bool OsSoftTimerStart(void* pTimerHandle, uint32_t ulTimeToWait);
bool OsSoftTimerStop(void* pTimerHandle);
bool OsSoftTimerStart(void* pTimerHandle);
bool OsGetSoftTimerTime(void *pTimerHandle, uint32_t *pulTimeInMs);
bool OsFactCreateThread(_sOsFactThreadAttributes	*psTaskAttributes, void *pvTaskRoutine, void *pvArgs);
bool OsCreateMsgQ(void **pvQueueHandle, uint32_t ulQElmntSize, uint32_t ulQElmntCnt);
bool OsRecvMsgQ(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks);
bool OsSendMsgQ(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks);
void OsTaskDelay(uint32_t ulDelayMs);

#endif /* __OS_FACTORY_H__ */
