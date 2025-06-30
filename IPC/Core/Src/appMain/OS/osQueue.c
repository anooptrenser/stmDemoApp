//**************************** OsQueue *****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for OS queue wrapper functions.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include "osQueue.h"
#include "cmsis_os2.h"

osMessageQueueId_t PollerToReceiverQueueHandle;
osMessageQueueId_t ReceiverToPollerQueueHandle;

//******************************.OsQueueSendRequest.***************************
// Purpose : Sends a request message to the PollerToReceiver queue.
// Inputs  : msg - Pointer to the request message.
// Outputs : None
// Return  : osStatus_t - Status of the queue put operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueSendRequest(RequestMessage *msg)
{
    return osMessageQueuePut(PollerToReceiverQueueHandle, msg, 0, 0);
}

//******************************.OsQueueReceiveRequest.************************
// Purpose : Receives a request message from the PollerToReceiver queue.
// Inputs  : msg     - Pointer to the request message buffer.
//           timeout - Timeout value for the receive operation.
// Outputs : None
// Return  : osStatus_t - Status of the queue get operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueReceiveRequest(RequestMessage *msg, uint32_t timeout)
{
    return osMessageQueueGet(PollerToReceiverQueueHandle, msg, NULL, timeout);
}

//******************************.OsQueueSendAck.*******************************
// Purpose : Sends an acknowledgment message to the ReceiverToPoller queue.
// Inputs  : msg - Pointer to the acknowledgment message.
// Outputs : None
// Return  : osStatus_t - Status of the queue put operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueSendAck(AckMessage *msg)
{
    return osMessageQueuePut(ReceiverToPollerQueueHandle, msg, 0, 0);
}

//******************************.OsQueueReceiveAck.****************************
// Purpose : Receives an acknowledgment message from the ReceiverToPoller queue.
// Inputs  : msg     - Pointer to the acknowledgment message buffer.
//           timeout - Timeout value for the receive operation.
// Outputs : None
// Return  : osStatus_t - Status of the queue get operation.
// Notes   : None
//*****************************************************************************
osStatus_t OsQueueReceiveAck(AckMessage *msg, uint32_t timeout)
{
    return osMessageQueueGet(ReceiverToPollerQueueHandle, msg, NULL, timeout);
}

//EOF
