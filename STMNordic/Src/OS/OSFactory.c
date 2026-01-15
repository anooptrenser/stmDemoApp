//******************************* OS_FACTORY *********************************
// Copyright (c) 2025
// All Rights Reserved
//*****************************************************************************
//
// File     : OsFactory.c
// Summary  : File containing OS functions
// Note     : None
// Author   : None
// Date     : 12/06/2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "OsFactory.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Extern Variables ******************************

//****************************** Local Functions ******************************

//***************************** Local Variables *******************************

//*************************** Function Definition *****************************

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Create event Handle
//Inputs    : pOsEventsHandle: Event group handle
//Outputs   : None
//Return    : bRetVal: Returs true if event handle is created, else false
//Notes     :
//*****************************************************************************
bool OsCreateEventGroup(EventGroupHandle_t* pOsEventsHandle)
{
	bool bRetVal = false;

	if (pOsEventsHandle)
	{
		*pOsEventsHandle = xEventGroupCreate();
		bRetVal = true;
	}

	return bRetVal;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Set event bits from ISR
//Inputs    : pOsEventsHandle: Event group handle
//Inputs    : ulSetEventBits: Event bits to set
//Outputs   : None
//Return    : bRetVal: Returs true if event bit is set, else false
//Notes     :
//*****************************************************************************
bool OsSetEventBitsFromISR(EventGroupHandle_t* pOsEventsHandle, uint32_t ulSetEventBits)
{
	bool bRetVal = false;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (pdFAIL != xEventGroupSetBitsFromISR(pOsEventsHandle, ulSetEventBits, &xHigherPriorityTaskWoken))
	{
		bRetVal = true;
	}

	return bRetVal;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Get event bits
//Inputs    : pOsEventsHandle: Event group handle
//Outputs   : None
//Return    : ulGetEventBit: returs event bit from ISR
//Notes     :
//*****************************************************************************
uint32_t OsGetEventBits(EventGroupHandle_t* pOsEventsHandle)
{
	uint32_t ulGetEventBits = 0;

	ulGetEventBits = xEventGroupGetBits(*pOsEventsHandle);

	return ulGetEventBits;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Clear event bits
//Inputs    : pOsEventsHandle: Event group handle
//Inputs    : ulClearEventBits: Bits to clear
//Outputs   : None
//Return    : bRetVal: Returs true if event bit is set, else false
//Notes     :
//*****************************************************************************
bool OsClearEventBits(EventGroupHandle_t* pOsEventsHandle, uint32_t ulClearEventBits)
{
	bool bRetVal = false;

	if (pdFAIL != xEventGroupClearBits(*pOsEventsHandle, ulClearEventBits))
	{
		bRetVal = true;
	}

	return bRetVal;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Set event bits
//Inputs    : pOsEventsHandle: Event group handle
//Inputs    : ulSetEventBits: Bits to set
//Outputs   : None
//Return    : bRetVal: Returs true if event bit is set, else false
//Notes     :
//*****************************************************************************
bool OsSetEventBits(EventGroupHandle_t* pOsEventsHandle, uint32_t ulSetEventBits)
{
	bool bRetVal = false;
	EventBits_t  ulEvents = 0;

	ulEvents = xEventGroupSetBits(*pOsEventsHandle, ulSetEventBits);

	if ((ulEvents & ulSetEventBits) == ulSetEventBits)
	{
		bRetVal = true;
	}

	return bRetVal;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Wait for an event
//Inputs    : pOsEventsHandle: Event group handle
//Inputs    : ulWaitTimeout: wait Timeout
//Inputs    : ulEvents - Events to wait
//Inputs    : bClearOnexit - Clear event on exit
//Outputs   : None
//Return    : Returns event bits set.
//Notes     :
//*****************************************************************************
uint32_t  OsWaitEvent(EventGroupHandle_t* pOsEventsHandle, uint32_t ulWaitTimeout,
		uint32_t ulEvents, bool bClearOnexit)
{
	return xEventGroupWaitBits(*pOsEventsHandle, ulEvents, bClearOnexit, pdFALSE, ulWaitTimeout);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Create a new Software Timer
// Inputs  : pucTimerName - Name of Timer
// Inputs  : ulTimePeriod - Time period in ms
// Inputs  : bAutoReload - Timer Auto reload enable or disable
// Inputs  : pTimerId - Id of Timer
// Inputs  : pCallBack - Call back function to be called when timer expires
// Outputs : pTimerHandle - Timer Handle
// Returns : True if Timer creation was success
// Notes   : This function for creates new task in RTOS
//*****************************************************************************
static bool FactoryOsSoftTimerCreate(void** pTimerHandle, uint8_t* pucTimerName, uint32_t ulTimePeriod,
							bool bAutoReload, void* pTimerId, void* pCallBack)
{
	bool bReturn = false;

	if(pucTimerName && pCallBack && pTimerHandle)
	{
		(*pTimerHandle) = xTimerCreate ((char*)pucTimerName, (TickType_t)ulTimePeriod, bAutoReload, pTimerId, pCallBack);

		if(*pTimerHandle)
		{
			bReturn = true;
		}
	}

	return bReturn;
}

// /*-----------------------------------------------------------------------------
// Purpose : To start soft timer
// Inputs  : pTimerHandle - Pointer to Timer Handle
// Inputs  : ulTimeToWait - Time to wait
// Outputs : NULL
// Returns : True if success, False if fails
// Notes   : None
// -----------------------------------------------------------------------------*/
// static bool FactoryOsSoftTimerStart(void* pTimerHandle, uint32_t ulTimeToWait)
// {
// 	bool bReturn = false;

// 	if(pTimerHandle)
// 	{
// 		if (pdPASS == xTimerChangePeriod ((TimerHandle_t)pTimerHandle, ulTimeToWait/portTICK_PERIOD_MS, 0))
// 		{
// 			bReturn=true;
// 		}
// 	}

// 	return bReturn;
// }

//******************************.FUNCTION_HEADER.******************************
// Purpose : To stop soft timer
// Inputs  : pTimerHandle - Timer Handle
// Outputs : None
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
static bool FactoryOsSoftTimerStop(void* pTimerHandle)
{
	bool bReturn = false;

	if(pTimerHandle)
	{
		if (pdPASS == xTimerStop ((TimerHandle_t)pTimerHandle, 0))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : To start soft timer
// Inputs  : pTimerHandle - Timer Handle
// Outputs : NULL
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
static bool FactoryOsSoftTimerStart(void* pTimerHandle)
{
	bool bReturn = false;

	if(pTimerHandle)
	{
		if (pdPASS == xTimerStart ((TimerHandle_t)pTimerHandle, 0))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : To get soft timer time
// Inputs  : pTimerHandle - Timer Handle
// Outputs : pulTimeInMs - Time in ms
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
static bool FactoryGetOsSoftTimerTime(void* pTimerHandle, uint32_t *pulTimeInMs)
{
	bool bReturn = false;

	if(pTimerHandle)
	{
		*pulTimeInMs = xTimerGetPeriod((TimerHandle_t)pTimerHandle);
		bReturn = true;
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Create a new Software Timer
// Inputs  : pucTimerName - Name of Timer
// Inputs  : ulTimePeriod - Time period in Ticks
// Inputs  : bAutoReload - Timer Auto reload enable or disable
// Inputs  : pTimerId - Id of Timer
// Inputs  : pCallBack - Call back function to be called when timer expires
// Outputs : pTimerHandle - Timer Handle
// Returns : True if Timer creation was success
// Notes   : This function for creates new task in RTOS
//*****************************************************************************
bool OsSoftTimerCreate(void** pTimerHandle, uint8_t* pucTimerName, uint32_t ulTimePeriod,
							bool bAutoReload, void* pTimerId, void* pCallBack)
{
	bool bReturn = false;

	if(pucTimerName && pCallBack && pTimerHandle)
	{

		bReturn = FactoryOsSoftTimerCreate(pTimerHandle, pucTimerName,
						ulTimePeriod, bAutoReload, pTimerId, pCallBack);
	}

	return bReturn;
}

// /*-----------------------------------------------------------------------------
// Purpose : To start soft timer
// Inputs  : pTimerHandle - Timer Handle
// Inputs  : ulTimeToWait - Time to wait
// Outputs : NULL
// Returns : True if success, False if fails
// Notes   : None
// -----------------------------------------------------------------------------*/
// bool OsSoftTimerStart(void* pTimerHandle, uint32_t ulTimeToWait)
// {
// 	bool bReturn = false;

// 	if(pTimerHandle)
// 	{
// 		bReturn  = FactoryOsSoftTimerStart(pTimerHandle, ulTimeToWait);
// 	}

// 	return bReturn;
// }

//******************************.FUNCTION_HEADER.******************************
// Purpose : To stop soft timer
// Inputs  : pTimerHandle - Timer Handle
// Outputs : NULL
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsSoftTimerStop(void* pTimerHandle)
{
	bool bReturn = false;

	if(pTimerHandle)
	{
		bReturn  = FactoryOsSoftTimerStop(pTimerHandle);
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : To start soft timer
// Inputs  : pTimerHandle - Timer Handle
// Outputs : NULL
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsSoftTimerStart(void* pTimerHandle)
{
	bool bReturn = false;

	if(pTimerHandle)
	{
		bReturn  = FactoryOsSoftTimerStart(pTimerHandle);
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : To get soft timer time
// Inputs  : pTimerHandle - Timer Handle
// Outputs : pulTimeInMs -  Time in ms
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsGetSoftTimerTime(void *pTimerHandle, uint32_t *pulTimeInMs)
{
	bool bReturn = false;

	if(pTimerHandle && pulTimeInMs)
	{
		bReturn = FactoryGetOsSoftTimerTime(pTimerHandle, pulTimeInMs);
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Create task
// Inputs  : psTaskAttributes - Task attribute
// Inputs  : pvTaskRoutine - Task routine
// Inputs  : pvArgs - Task args
// Outputs : NOne
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsFactCreateThread(_sOsFactThreadAttributes	*psTaskAttributes, void *pvTaskRoutine, void *pvArgs)
{
	bool bReturn = false;

	if(psTaskAttributes && pvTaskRoutine)
	{
		if (xTaskCreate(pvTaskRoutine,
				psTaskAttributes->pcThreadName,
				psTaskAttributes->nThreadStackSize,
				pvArgs,
				psTaskAttributes->eThreadPriority,
				NULL))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Create message queue
// Inputs  : pQueueHandle - Queue handle
// Inputs  : ulQElmntSize - Q element size
// Inputs  : ulQElmntCnt - Q element count
// Outputs : None
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsCreateMsgQ(void **pvQueueHandle, uint32_t ulQElmntSize, uint32_t ulQElmntCnt)
{
	bool bReturn = false;

	if (pvQueueHandle)
	{
		*pvQueueHandle = xQueueCreate(ulQElmntCnt, ulQElmntSize);

		if (*pvQueueHandle)
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Receive message queue
// Inputs  : pvBuffer - Buffer to receive
// Inputs  : pQueueHandle - Q Handle
// Inputs  : ulWaitTicks - Q wait ticks
// Outputs : None
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsRecvMsgQ(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks)
{
	bool bReturn = false;

	if (pvQueueHandle && pvBuffer)
	{
		if (pdPASS == xQueueReceive(pvQueueHandle, pvBuffer, ulWaitTicks))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Send message queue
// Inputs  : pvBuffer - Buffer to receive
// Inputs  : pQueueHandle - Q Handle
// Inputs  : ulWaitTicks - Q wait ticks
// Outputs : None
// Returns : True if success, False if fails
// Notes   : None
//*****************************************************************************
bool OsSendMsgQ(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks)
{
	bool bReturn = false;

	if (pvQueueHandle && pvBuffer)
	{
		if (pdPASS == xQueueSend(pvQueueHandle, pvBuffer, ulWaitTicks))
		{
			bReturn = true;
		}
	}

	return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Os task delay in ms
// Inputs  : ulDelayMs	: Delay in ms
// Outputs : None
// Returns : None
// Notes   : None
//*****************************************************************************
void OsTaskDelay(uint32_t ulDelayMs)
{
	vTaskDelay(ulDelayMs);
}

//EOF
