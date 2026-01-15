//******************************.OSQueue.**************************************
// File     : OSQueue.c
// Summary  : RTOS queue wrapper implementation
// Author   :
// Date     : 21/07/2025
//*****************************************************************************
//*********************Include Files*******************************************
#include "OSQueue.h"
#include "OSFactory.h"

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Create RTOS queue
//Inputs    : ppvQueueHandle - pointer to queue handle
//            ulQElmntSize   - element size
//            ulQElmntCnt    - element count
//Outputs   : None
//Return    : true if success else false
//Notes     : None
//*****************************************************************************
bool OSQueueCreate(void **pvQueueHandle, uint32_t ulQElmntSize, uint32_t ulQElmntCnt)
{
    return OsCreateMsgQ(pvQueueHandle, ulQElmntSize, ulQElmntCnt);
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Send to RTOS queue
//Inputs    : pvQueueHandle - queue handle
//            pvBuffer      - buffer to send
//            ulWaitTicks   - wait ticks
//Outputs   : None
//Return    : true if success else false
//Notes     : None
//*****************************************************************************
bool OSQueueSend(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks)
{
    return OsSendMsgQ(pvQueueHandle, pvBuffer, ulWaitTicks);
}

//******************************.FUNCTION_HEADER.******************************
//Purpose   : Receive from RTOS queue
//Inputs    : pvQueueHandle - queue handle
//            pvBuffer      - buffer to receive
//            ulWaitTicks   - wait ticks
//Outputs   : None
//Return    : true if success else false
//Notes     : None
//*****************************************************************************
bool OSQueueRecv(void *pvQueueHandle, void *pvBuffer, uint32_t ulWaitTicks)
{
    return OsRecvMsgQ(pvQueueHandle, pvBuffer, ulWaitTicks);
}

//EOF
