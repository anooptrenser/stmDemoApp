//**************************** OsQueue ****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for OS queue wrapper functions.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdbool.h>
#include "osQueue.h"
#include "cmsis_os2.h"

//******************************* Global Variables ****************************
osMessageQueueId_t pPollerToReceiverQueueHandle = NULL;
osMessageQueueId_t pReceiverToPollerQueueHandle = NULL;

//******************************.OsQueueSendRequest.***************************
// Purpose : Sends a request message to the PollerToReceiver queue.
// Inputs  : pstRequestMessage - Pointer to the request message.
// Outputs : None
// Return  : true  - if the message was successfully sent to the queue.
//           false - if the queue handle is invalid or send failed.
// Notes   : Application layer abstraction; hides RTOS details.
//*****************************************************************************
bool OsQueueSendRequest(REQUEST_MESSAGE* pstRequestMessage)
{
    if ((pstRequestMessage == NULL) || (pPollerToReceiverQueueHandle == NULL))
    {
        return false;
    }

    osStatus_t enStatus = osMessageQueuePut(pPollerToReceiverQueueHandle, 
        pstRequestMessage, 0, 0);

    return (enStatus == osOK) ? true : false;
}

//******************************.OsQueueReceiveRequest.************************
// Purpose : Receives a request message from the PollerToReceiver queue.
// Inputs  : pstRequestMessage - Pointer to the request message buffer.
//           ulTimeout         - Timeout value for the receive operation.
// Outputs : None
// Return  : true  - if a message was successfully received from the queue.
//           false - if the queue handle is invalid or receive failed.
// Notes   : Application layer abstraction; hides RTOS details.
//*****************************************************************************
bool OsQueueReceiveRequest(REQUEST_MESSAGE* pstRequestMessage, uint32 ulTimeout)
{
    if ((pstRequestMessage == NULL) || (pPollerToReceiverQueueHandle == NULL))
    {
        return false;
    }

    osStatus_t enStatus = osMessageQueueGet(pPollerToReceiverQueueHandle, 
        pstRequestMessage, NULL, ulTimeout);

    return (enStatus == osOK) ? true : false;
}

//******************************.OsQueueSendAck.*******************************
// Purpose : Sends an acknowledgment message to the ReceiverToPoller queue.
// Inputs  : pstAckMessage - Pointer to the acknowledgment message.
// Outputs : None
// Return  : true  - if the message was successfully sent to the queue.
//           false - if the queue handle is invalid or send failed.
// Notes   : Application layer abstraction; hides RTOS details.
//*****************************************************************************
bool OsQueueSendAck(ACK_MESSAGE* pstAckMessage)
{
    if ((pstAckMessage == NULL) || (pReceiverToPollerQueueHandle == NULL))
    {
        return false;
    }

    osStatus_t enStatus = osMessageQueuePut(pReceiverToPollerQueueHandle, 
        pstAckMessage, 0, 0);

    return (enStatus == osOK) ? true : false;
}

//******************************.OsQueueReceiveAck.****************************
// Purpose : Receives an acknowledgment message from the ReceiverToPoller queue.
// Inputs  : pstAckMessage - Pointer to the acknowledgment message buffer.
//           ulTimeout     - Timeout value for the receive operation.
// Outputs : None
// Return  : true  - if a message was successfully received from the queue.
//           false - if the queue handle is invalid or receive failed.
// Notes   : Application layer abstraction; hides RTOS details.
//*****************************************************************************
bool OsQueueReceiveAck(ACK_MESSAGE* pstAckMessage, uint32 ulTimeout)
{
    if ((pstAckMessage == NULL) || (pReceiverToPollerQueueHandle == NULL))
    {
        return false;
    }

    osStatus_t enStatus = osMessageQueueGet(pReceiverToPollerQueueHandle, 
        pstAckMessage, NULL, ulTimeout);

    return (enStatus == osOK) ? true : false;
}

//EOF
