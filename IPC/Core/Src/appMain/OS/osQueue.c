//**************************** OsQueue *****************************************
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
#include "osQueue.h"
#include "cmsis_os2.h"

//******************************* Global Variables ****************************
osMessageQueueId_t pPollerToReceiverQueueHandle = NULL;
osMessageQueueId_t pReceiverToPollerQueueHandle = NULL;

//******************************.OsQueueSendRequest.***************************
// Purpose : Sends a request message to the PollerToReceiver queue.
// Inputs  : pstMessage - Pointer to the request message.
// Outputs : None
// Return  : osStatus_t - Status of the queue put operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueSendRequest(REQUEST_MESSAGE* pstMessage)
{
    if (pPollerToReceiverQueueHandle == NULL)
    {
        return osErrorParameter;
    }

    return osMessageQueuePut(pPollerToReceiverQueueHandle, pstMessage, 0, 0);
}

//******************************.OsQueueReceiveRequest.************************
// Purpose : Receives a request message from the PollerToReceiver queue.
// Inputs  : pstMessage - Pointer to the request message buffer.
//           ulTimeout  - Timeout value for the receive operation.
// Outputs : None
// Return  : osStatus_t - Status of the queue get operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueReceiveRequest(REQUEST_MESSAGE* pstMessage, uint32 ulTimeout)
{
    if (pPollerToReceiverQueueHandle == NULL)
    {
        return osErrorParameter;
    }

    return osMessageQueueGet(pPollerToReceiverQueueHandle, pstMessage, NULL, ulTimeout);
}

//******************************.OsQueueSendAck.*******************************
// Purpose : Sends an acknowledgment message to the ReceiverToPoller queue.
// Inputs  : pstAck - Pointer to the acknowledgment message.
// Outputs : None
// Return  : osStatus_t - Status of the queue put operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueSendAck(ACK_MESSAGE* pstAck)
{
    if (pReceiverToPollerQueueHandle == NULL)
    {
        return osErrorParameter;
    }

    return osMessageQueuePut(pReceiverToPollerQueueHandle, pstAck, 0, 0);
}

//******************************.OsQueueReceiveAck.****************************
// Purpose : Receives an acknowledgment message from the ReceiverToPoller queue.
// Inputs  : pstAck   - Pointer to the acknowledgment message buffer.
//           ulTimeout - Timeout value for the receive operation.
// Outputs : None
// Return  : osStatus_t - Status of the queue get operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueReceiveAck(ACK_MESSAGE* pstAck, uint32 ulTimeout)
{
    if (pReceiverToPollerQueueHandle == NULL)
    {
        return osErrorParameter;
    }

    return osMessageQueueGet(pReceiverToPollerQueueHandle, pstAck, NULL, ulTimeout);
}

//EOF
