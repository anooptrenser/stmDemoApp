//**************************** ReceiverTasks ***********************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for Receiver Task with function table for CMD handling.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#include "receiverTasks.h"
#include "osQueue.h"
#include "gpioWrite.h"
#include <stdio.h>
#include <stdbool.h>

// Forward declarations for handler functions
static void HandleCmdGet(const RequestMessage *req, AckMessage *ack);
static void HandleCmdSet(const RequestMessage *req, AckMessage *ack);
static void HandleCmdClear(const RequestMessage *req, AckMessage *ack);

// Function pointer type for command handlers
typedef void (*CmdHandler)(const RequestMessage *, AckMessage *);

// Function table indexed by CommandType
static CmdHandler cmdHandlerTable[] = {
    NULL,               // CMD_ACK (not handled as a request)
    HandleCmdGet,       // CMD_GET
    HandleCmdSet,       // CMD_SET
    HandleCmdClear      // CMD_CLEAR
};

//******************************.ReceiverTaskCreate.****************************
bool ReceiverTaskCreate(void)
{
    const osThreadAttr_t ReceiverTask_attributes =
    {
        .name = "ReceiverTask",
        .stack_size = RECEIVER_TASK_STACK_SIZE,
        .priority = RECEIVER_TASK_PRIORITY,
    };

    osThreadId_t receiverTaskId = osThreadNew(ReceiverTaskRun, NULL, &ReceiverTask_attributes);

    if (NULL == receiverTaskId)
    {
        printf("Error: Failed to create ReceiverTask thread\r\n");
        return false;
    }

    return true;
}

//******************************.ReceiverTaskRun.*******************************
void ReceiverTaskRun(void *argument)
{
    RequestMessage receivedRequest;
    osStatus_t status;

    printf("ReceiverTask started\r\n");

    for (;;)
    {
        status = OsQueueReceiveRequest(&receivedRequest, osWaitForever);

        if (osOK == status)
        {
            printf("Received request - UID: %lu, CMD: %d, DATA: 0x%08lX\r\n",
                   receivedRequest.uid, receivedRequest.cmd, receivedRequest.data);

            AckMessage ack =
            {
                .uid = receivedRequest.uid,
                .cmd = CMD_ACK,
                .state = STATE_OK,
                .data = 0
            };

            // Use function table for command dispatch
            if (receivedRequest.cmd < (sizeof(cmdHandlerTable)/sizeof(cmdHandlerTable[0]))
                && cmdHandlerTable[receivedRequest.cmd] != NULL)
            {
                cmdHandlerTable[receivedRequest.cmd](&receivedRequest, &ack);
            }
            else
            {
                ack.state = STATE_ERROR;
                printf("Unknown CMD received: %d\r\n", receivedRequest.cmd);
            }

            if (osOK != OsQueueSendAck(&ack))
            {
                printf("Error: Failed to send acknowledgment message\r\n");
            }
        }
        else
        {
            printf("Error: Failed to receive request message (status: %d)\r\n", status);
        }
    }
}

//******************************.HandleCmdGet.**********************************
static void HandleCmdGet(const RequestMessage *req, AckMessage *ack)
{
    uint8_t ledState = GpioRead(LED_PIN, LED_PORT);
    ack->data = ledState;
    printf("CMD_GET: LED state is %u\r\n", ledState);
}

//******************************.HandleCmdSet.**********************************
static void HandleCmdSet(const RequestMessage *req, AckMessage *ack)
{
    // If data==1, turn ON; if data==0, turn OFF
    if (req->data)
    {
        GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_SET);
        ack->data = 1;
        printf("CMD_SET: LED set to ON\r\n");
    }
    else
    {
        GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_RESET);
        ack->data = 0;
        printf("CMD_SET: LED set to OFF\r\n");
    }
}

//******************************.HandleCmdClear.********************************
static void HandleCmdClear(const RequestMessage *req, AckMessage *ack)
{
    GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_RESET);
    ack->data = 0;
    printf("CMD_CLEAR: LED set to OFF\r\n");
}

//EOF
