//**************************** PollerTasks *************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for Poller Task and related GPIO/queue operations.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include "pollerTasks.h"
#include "osQueue.h"
#include "gpioWrite.h"
#include <stdio.h>
#include <stdbool.h>

//******************************* Global variables ****************************
static uint32_t transactionUID = 0;
static uint8_t toggle = 0;

//******************************.PollerTaskCreate.*****************************
// Purpose : Creates and starts the Poller Task for button polling.
// Inputs  : None
// Outputs : None
// Return  : true  - if thread creation was successful
//           false - if thread creation failed
// Notes   : Handles edge cases such as thread creation failure.
//*****************************************************************************
bool PollerTaskCreate(void)
{
    PollerToReceiverQueueHandle = osMessageQueueNew(
                                    POLLER_TO_RECEIVER_QUEUE_SIZE,
                                    sizeof(RequestMessage),
                                    NULL);
 
    const osThreadAttr_t PollerTask_attributes =
    {
        .name = "PollerTask",
        .stack_size = POLLER_TASK_STACK_SIZE,
        .priority = osPriorityHigh,
    };

    if (0 == PollerTask_attributes.stack_size)
    {
        printf("Error: PollerTask stack size is zero\r\n");
        return false;
    }

    osThreadId_t pollerTaskId = osThreadNew(PollerTaskRun, NULL, 
                                            &PollerTask_attributes);

    if (NULL == pollerTaskId)
    {
        printf("Error: Failed to create PollerTask thread\r\n");
        return false;
    }

    return true;
}

//******************************.PollerTaskRun.********************************
// Purpose : Main loop for the Poller Task. Polls the user button and sends a 
//           request message when a button press is detected.
// Inputs  : void *argument - Not used.
// Outputs : None
// Notes   : Handles button state and queue send edge cases.
//*****************************************************************************
void PollerTaskRun(void *argument)
{
    uint32_t lastButtonState = 0;
    uint32_t currentButtonState = 0;
    static uint32_t transactionUID = 0;
    static uint8_t ledOn = 0;
    static uint8_t toggle = 0; // Toggle between CMD_SET and CMD_GET

    printf("PollerTask started\r\n");

    for (;;)
    {
        currentButtonState = GpioRead(USER_BUTTON_PIN, USER_BUTTON_PORT);

        if ((currentButtonState != 0) && (currentButtonState != 1))
        {
            printf("Warning: Invalid button state read: %lu\r\n", currentButtonState);
            continue;
        }

        // Detect falling edge (button press)
        if ((lastButtonState == 1) && (currentButtonState == 0))
        {
            printf("Button pressed - sending request\r\n");

            RequestMessage msg;

            if (toggle == 0)
            {
                // Send CMD_SET to toggle LED state
                msg.uid = transactionUID++;
                msg.cmd = CMD_SET;
                msg.data = ledOn; // 1 = ON, 0 = OFF
                printf("Poller: Sending CMD_SET, data=%u (LED %s)\r\n", ledOn, ledOn ? "ON" : "OFF");
                ledOn ^= 1; // Toggle for next press
            }
            else
            {
                // Send CMD_GET to query LED state
                msg.uid = transactionUID++;
                msg.cmd = CMD_GET;
                msg.data = 0; // Not used for GET
                printf("Poller: Sending CMD_GET\r\n");
            }

            toggle ^= 1; // Alternate between CMD_SET and CMD_GET

            if (osOK != OsQueueSendRequest(&msg))
            {
                printf("Error: Failed to send request message to queue\r\n");
            }
        }

        // ---- ACK CONSUMPTION SECTION ----
        // Try to read all available ACKs from the queue (non-blocking)
        AckMessage ack;
        while (OsQueueReceiveAck(&ack, 0) == osOK)
        {
            printf("Poller: Received ACK - UID: %lu, STATE: %d, DATA: 0x%08lX\r\n",
                   ack.uid, ack.state, ack.data);
        }
        // ---------------------------------

        lastButtonState = currentButtonState;
        osDelay(POLLER_TASK_DELAY_MS);
    }
}

//EOF
