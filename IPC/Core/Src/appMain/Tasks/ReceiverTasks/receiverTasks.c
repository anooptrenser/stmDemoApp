//**************************** ReceiverTasks **********************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for Receiver Task with function table for CMD 
//              handling.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "receiverTasks.h"
#include "osQueue.h"
#include "gpioWrite.h"
#include <stdio.h>
#include <stdbool.h>

//******************************* Local Function Prototypes *******************
static void ReceiverCmdGet(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck);
static void ReceiverCmdSet(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck);
static void ReceiverCmdClear(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck);

static void ProcessReceivedRequest(const REQUEST_MESSAGE* pstReq);
static void DispatchCommand(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck);
static void SendAckMessage(const ACK_MESSAGE* pstAck);

// Function pointer table for command handlers
static CMD_HANDLER CmdHandlerTable[] =
{
    NULL,           // CMD_ACK (not handled as a request)
    ReceiverCmdGet,   // CMD_GET
    ReceiverCmdSet,   // CMD_SET
    ReceiverCmdClear  // CMD_CLEAR
};

//******************************.ReceiverTaskCreate.***************************
// Purpose : Creates and starts the Receiver Task.
// Inputs  : None
// Outputs : None
// Return  : true  - if the message queue and thread creation were successful
//           false - if the message queue or thread creation failed
// Notes   : Initializes a message queue for inter-task communication and
//           creates the ReceiverTaskRun thread.
//*****************************************************************************
bool ReceiverTaskCreate(void)
{
    const osThreadAttr_t ReceiverTaskAttributes =
    {
        .name = "ReceiverTask",
        .stack_size = RECEIVER_TASK_STACK_SIZE,
        .priority = RECEIVER_TASK_PRIORITY,
    };

	pReceiverToPollerQueueHandle = osMessageQueueNew(
        RECEIVER_TO_POLLER_QUEUE_SIZE,
        sizeof(ACK_MESSAGE),
        NULL
    );

    osThreadId_t ulReceiverTaskId = osThreadNew(
        ReceiverTaskRun,
        NULL,
        &ReceiverTaskAttributes
    );

    if (NULL == ulReceiverTaskId)
    {
        printf("Error: Failed to create ReceiverTask thread\r\n");
        return false;
    }

    return true;
}

//******************************.ReceiverTaskRun.******************************
// Purpose : Main execution loop for the Receiver Task.
// Inputs  : pvArgument - Pointer to arguments (not used in this task)
// Outputs : None
// Notes   : Receives and processes request messages in a modular way.
//*****************************************************************************
void ReceiverTaskRun(void* pvArgument)
{
    printf("ReceiverTask started\r\n");

    for (;;)
    {
        REQUEST_MESSAGE stReceivedRequest;
        osStatus_t status = OsQueueReceiveRequest(&stReceivedRequest, osWaitForever);

        if (osOK == status)
        {
            ProcessReceivedRequest(&stReceivedRequest);
        }
        else
        {
            printf("Error: Failed to receive request message (status: %d)\r\n",
                             status);
        }
    }
}

//******************************.ProcessReceivedRequest.************************
// Purpose : Processes a received request message.
// Inputs  : pstReq - Pointer to the received REQUEST_MESSAGE
// Outputs : None
//*****************************************************************************
static void ProcessReceivedRequest(const REQUEST_MESSAGE* pstReq)
{
    ACK_MESSAGE stAck =
    {
        .ulUid = pstReq->ulUid,
        .ucCmd = CMD_ACK,
        .ucState = STATE_OK,
        .ulData = 0
    };

	printf("Received request - UID: %lu, CMD: %d, DATA: 0x%08lX\r\n",
        pstReq->ulUid, pstReq->ucCmd, pstReq->ulData);

    DispatchCommand(pstReq, &stAck);
    SendAckMessage(&stAck);
}

//******************************.DispatchCommand.******************************
// Purpose : Dispatches the command to the appropriate handler using the
//           function table.
// Inputs  : pstReq - Pointer to the received REQUEST_MESSAGE
//           pstAck - Pointer to the ACK_MESSAGE to be populated
// Outputs : Populates the ACK_MESSAGE based on command handling.
//*****************************************************************************
static void DispatchCommand(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck)
{
    uint8 ucCmdIndex = pstReq->ucCmd;

    if (pstReq == NULL || pstAck == NULL)
    {
        printf("Error: Null pointer received in DispatchCommand\r\n");
        pstAck->ucState = STATE_ERROR;
        return;
    }

    if (ucCmdIndex < (sizeof(CmdHandlerTable) / sizeof(CmdHandlerTable[0])) &&
        CmdHandlerTable[ucCmdIndex] != NULL)
    {
        CmdHandlerTable[ucCmdIndex](pstReq, pstAck);
    }
    else
    {
        pstAck->ucState = STATE_ERROR;
        printf("Unknown CMD received: %d\r\n", pstReq->ucCmd);
    }
}

//******************************.SendAckMessage.*******************************
// Purpose : Sends the acknowledgment message to the queue.
// Inputs  : pstAck - Pointer to the ACK_MESSAGE to be sent
// Outputs : None
//*****************************************************************************
static void SendAckMessage(const ACK_MESSAGE* pstAck)
{
    if (osOK != OsQueueSendAck((ACK_MESSAGE*)pstAck))
    {
        printf("Error: Failed to send acknowledgment message\r\n");
    }
}

//******************************.ReceiverCmdGet.*******************************
// Purpose : Handles the CMD_GET command.
// Inputs  : pstReq - Pointer to the received REQUEST_MESSAGE
//           pstAck - Pointer to the ACK_MESSAGE to be populated
// Outputs : Populates the 'pstAck->ulData' with the current LED state.
//*****************************************************************************
static void ReceiverCmdGet(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck)
{
    (void)pstReq; // Unused parameter

    if (pstAck == NULL)
    {
        printf("Error: Null pointer received in HandleCmdGet\r\n");
        return;
    }

    uint8 ucLedState = GpioRead(LED_PIN, LED_PORT);
    pstAck->ulData = ucLedState;
    printf("CMD_GET: LED state is %u\r\n", ucLedState);
}

//******************************.ReceiverCmdSet.*******************************
// Purpose : Handles the CMD_SET command.
// Inputs  : pstReq - Pointer to the received REQUEST_MESSAGE
//           pstAck - Pointer to the ACK_MESSAGE to be populated
// Outputs : Controls the LED state and populates 'pstAck->ulData' with the 
//           new LED state.
//*****************************************************************************
static void ReceiverCmdSet(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck)
{
    if (pstReq->ulData)
    {
        GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_SET);
        pstAck->ulData = 1;
        printf("CMD_SET: LED set to ON\r\n");
    }
    else
    {
        GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_RESET);
        pstAck->ulData = 0;
        printf("CMD_SET: LED set to OFF\r\n");
    }
}

//******************************.ReceiverCmdClear.*****************************
// Purpose : Handles the CMD_CLEAR command.
// Inputs  : pstReq - Pointer to the received REQUEST_MESSAGE
//           pstAck - Pointer to the ACK_MESSAGE to be populated
// Outputs : Turns off the LED and populates 'pstAck->ulData' with the new 
//           LED state.
//*****************************************************************************
static void ReceiverCmdClear(const REQUEST_MESSAGE* pstReq, ACK_MESSAGE* pstAck)
{
    (void)pstReq; // Unused parameter

    if (pstAck == NULL)
    {
        printf("Error: Null pointer received in HandleCmdClear\r\n");
        return;
    }

    GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_RESET);
    pstAck->ulData = 0;
    printf("CMD_CLEAR: LED set to OFF\r\n");
}

//EOF
