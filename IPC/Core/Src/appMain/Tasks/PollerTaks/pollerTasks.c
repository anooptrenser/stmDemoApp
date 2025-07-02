//**************************** PollerTasks *************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for Poller Task and related GPIO/queue operations.
//  Note      : None
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
    const osThreadAttr_t stPollerTaskAttributes =
    {
        .name = "PollerTask",
        .stack_size = POLLER_TASK_STACK_SIZE,
        .priority = POLLER_TASK_PRIORITY,
    };

	pPollerToReceiverQueueHandle = osMessageQueueNew(
        POLLER_TO_RECEIVER_QUEUE_SIZE,
        sizeof(REQUEST_MESSAGE),
        NULL
    );

    osThreadId_t ulPollerTaskId = osThreadNew(
        PollerTaskRun,
        NULL,
        &stPollerTaskAttributes
    );

    if (NULL == ulPollerTaskId)
    {
        printf("Error: Failed to create PollerTask thread\r\n");
        return false;
    }

    return true;
}

//******************************.PollerTaskRun.********************************
// Purpose : Entry point for the poller task loop.
// Inputs  : void* pvArgument - Not used.
// Outputs : None
// Notes   : Calls helper routines for button polling and ACK processing.
//*****************************************************************************
void PollerTaskRun(void* argument)
{
    uint32 ulLastButtonState    = BUTTON_RELEASED_STATE;
    uint32 ulCurrentButtonState = BUTTON_RELEASED_STATE;
    static uint32 ulTransactionUid = 0;
    static uint8 ucLedOn        = 0;
    static uint8 ucToggle       = 0;

    printf("PollerTask started\r\n");

    for (;;)
    {
        ulCurrentButtonState = GpioRead(USER_BUTTON_PIN, USER_BUTTON_PORT);

        if (!IsButtonStateValid(ulCurrentButtonState))
        {
            printf("Warning: Invalid button state read: %lu\r\n", 
                                            ulCurrentButtonState);
            continue;
        }

        if (IsButtonPressed(ulLastButtonState, ulCurrentButtonState))
        {
            PollerButtonPress(&ulTransactionUid, &ucLedOn, &ucToggle);
        }

        ProcessAckMessages();

        ulLastButtonState = ulCurrentButtonState;
        osDelay(POLLER_TASK_DELAY_MS);
    }
}

//******************************.IsButtonStateValid.***************************
// Purpose : Checks if the button state is valid.
// Inputs  : ulButtonState
// Outputs : bool
//*****************************************************************************
bool IsButtonStateValid(uint32 ulButtonState)
{
    return (ulButtonState == BUTTON_PRESSED_STATE) ||
           (ulButtonState == BUTTON_RELEASED_STATE);
}

//******************************.IsButtonPressed.******************************
// Purpose : Detects falling edge (button press).
// Inputs  : ulLastState,  ulCurrentState
// Outputs : bool
//*****************************************************************************
bool IsButtonPressed(uint32 ulLastState, uint32 ulCurrentState)
{
    return (ulLastState == BUTTON_RELEASED_STATE) &&
           (ulCurrentState == BUTTON_PRESSED_STATE);
}

//******************************.HandleButtonPress.****************************
// Purpose : Handles the logic for button press event.
// Inputs  : pulTransactionUid, pucLedOn, pucToggle
// Outputs : None
//*****************************************************************************
void PollerButtonPress(uint32* pulTransactionUid, uint8* pucLedOn, uint8* pucToggle)
{
    REQUEST_MESSAGE stRequestMessage;

    if ((pulTransactionUid == NULL) || (pucLedOn == NULL) || (pucToggle == NULL))
    {
        printf("Error: Null pointer passed to HandleButtonPress\r\n");
        return;
    }

    printf("Button pressed - sending request\r\n");

    if (0 == *pucToggle)
    {
        stRequestMessage.ulUid  = (*pulTransactionUid)++;
        stRequestMessage.ucCmd  = CMD_SET;
        stRequestMessage.ulData = *pucLedOn;
        printf("Poller: Sending CmdSet, data=%u (LED %s)\r\n",
            *pucLedOn, *pucLedOn ? "ON" : "OFF");
        *pucLedOn ^= 1;
    }
    else
    {
        stRequestMessage.ulUid  = (*pulTransactionUid)++;
        stRequestMessage.ucCmd  = CMD_GET;
        stRequestMessage.ulData = 0;
        printf("Poller: Sending CmdGet\r\n");
    }

    *pucToggle ^= 1;

    if (osOK != OsQueueSendRequest(&stRequestMessage))
    {
        printf("Error: Failed to send request message to queue\r\n");
    }
}

//******************************.ProcessAckMessages.***************************
// Purpose : Processes all available ACK messages from the queue.
// Inputs  : None
// Outputs : None
//*****************************************************************************
void ProcessAckMessages(void)
{
    ACK_MESSAGE stAckMessage;

    while (OsQueueReceiveAck(&stAckMessage, 0) == osOK)
    {
        printf("Poller: Received ACK - UID: %lu, STATE: %d, DATA: 0x%08lX\r\n",
            stAckMessage.ulUid, stAckMessage.ucState, stAckMessage.ulData);
    }
}

//EOF
