//******************************* UartFrameReceiver **************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartFrameReceiverTasks.c
// Summary  : UART frame extraction RTOS thread (state machine, style-compliant)
// Author   : Anoop G 
// Date     : 27-07-2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include "CircularBuffer.h"
#include "Parser.h"
#include "OsFactory.h"
#include "UartFrameReceiverTasks.h"
#include "UartProtoBuilder.h"
#include "AppMain.h"
#include "OSQueue.h"
#include "Tmp.h"
#include "UartDriver.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
//******************************* Local Types ********************************* 

//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 

//****************************** Local Functions ******************************

//***************************** Global Variables ******************************
static uint8 sucFrameBuffer[MAX_RAW_FRAME_LEN] = {0};
static uint16 sunFrameIndex = 0;
static uint32 sulPayloadLen = 0;
static DATA_FRAME sPartialFrame = {0};  // Store parsed header to avoid redundant parsing
static UART_FRAME_RX_STATE seRxState = UART_FRAME_RX_WAIT_START;

//************************ Static Function Prototypes *************************
static void UartFrameHandleWaitStart(uint8 ucByte);
static void UartFrameHandleHeader(uint8 ucByte);
static void UartFrameHandlePayload(uint8 ucByte);
static void UartFrameHandleChecksum(uint8 ucByte);
static void UartFrameHandleStop(uint8 ucByte);
static void UartFrameProcessFull(void);
static bool UartFrameEnqueue(DATA_FRAME* psFrame);
static void UartFrameResetState(void);

//******************************.FUNCTION_HEADER.******************************
// Purpose  : UART frame receiver RTOS task (state machine, compact, style-compliant).
// Inputs   : pvArgs - Unused task parameter pointer (FreeRTOS standard).
// Outputs  : None
// Return   : None
// Notes    : This task continuously reads bytes from the UART receive buffer
//            and processes them according to the defined frame reception
//            state machine. It handles different stages of frame reception
//            (start, header, payload, checksum, stop) and includes
//            defensive mechanisms against buffer overflow.
//*****************************************************************************
void UartFrameReceiverTask(void *pvArgs)
{
    (void)pvArgs;
    uint8 ucReceivedByte = 0U;

    while (1)
    {
        if (!UartRxBufferPop(&gUartRxBuffer, &ucReceivedByte)) {
            OsTaskDelay(1);
            continue;
        }

        switch (seRxState)
        {
            case UART_FRAME_RX_WAIT_START:
            {
                UartFrameHandleWaitStart(ucReceivedByte);
                break;
            }

            case UART_FRAME_RX_HEADER:
            {
                UartFrameHandleHeader(ucReceivedByte);
                break;
            }

            case UART_FRAME_RX_PAYLOAD:
            {
                UartFrameHandlePayload(ucReceivedByte);
                break;
            }

            case UART_FRAME_RX_CHECKSUM:
            {
                UartFrameHandleChecksum(ucReceivedByte);
                break;
            }

            case UART_FRAME_RX_STOP:
            {
                UartFrameHandleStop(ucReceivedByte);
                break;
            }
            
            default:
            {
            	UartFrameResetState();
                break;
            }
        }

        // Defensive overflow/reset
        if (sunFrameIndex >= MAX_RAW_FRAME_LEN) 
        {
            printf("[UartFrameReceiver] Buffer overflow, resetting.\n");
            UartFrameResetState();
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Handle waiting for UART start byte in the frame reception state machine
// Inputs  : ucByte - Received byte from UART circular buffer
// Outputs : None
// Return  : None
// Notes   : Transitions to header state when start byte (0x7E) is detected
//******************************************************************************
static void UartFrameHandleWaitStart(uint8 ucByte)
{
    if (UART_START_BYTE == ucByte)
    {
        sucFrameBuffer[0] = ucByte;
        sunFrameIndex = 1;
        sulPayloadLen = 0;
        seRxState = UART_FRAME_RX_HEADER;
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Reset frame reception state machine to initial state
// Inputs   : None
// Outputs  : Clears all state variables and frame buffer
// Returns  : None
//*****************************************************************************
static void UartFrameResetState(void)
{
    seRxState = UART_FRAME_RX_WAIT_START;
    sunFrameIndex = 0;
    sulPayloadLen = 0;
    memset(&sPartialFrame, 0, sizeof(sPartialFrame));
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Handle reception of frame header bytes in the state machine
// Inputs  : ucByte - Received byte from UART circular buffer
// Outputs : None
// Return  : None
// Notes   : Parses header when complete, validates payload length, transitions
//           to payload/checksum state
//*****************************************************************************
static void UartFrameHandleHeader(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (sunFrameIndex == (FRAME_HEADER_SIZE + 1))
    {
        uint8* pucHeader = &sucFrameBuffer[1];
        ParseHeader(pucHeader, &sPartialFrame);  // Parse once and store
        sulPayloadLen = sPartialFrame.ulLength;

        if (sulPayloadLen > MAX_FRAME_SIZE) 
        {
            printf("[UartFrameReceiver] Payload too large, discarding.\n");
            seRxState = UART_FRAME_RX_WAIT_START;
            sunFrameIndex = 0;
            sulPayloadLen = 0;
            memset(&sPartialFrame, 0, sizeof(sPartialFrame));  // Clear partial frame
        } else 
        {
            seRxState = (sulPayloadLen > 0U) ?
                        UART_FRAME_RX_PAYLOAD : UART_FRAME_RX_CHECKSUM;
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Handle reception of payload bytes in the frame state machine
// Inputs  : ucByte - Received byte from UART circular buffer
// Outputs : None
// Return  : None
// Notes   : Accumulates payload bytes until expected length is reached, then 
//           transitions to checksum state
//******************************************************************************
static void UartFrameHandlePayload(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (sunFrameIndex == (FRAME_HEADER_SIZE + 1U + sulPayloadLen)) 
    {
        seRxState = UART_FRAME_RX_CHECKSUM;
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Handle reception of checksum byte in the frame state machine
// Inputs  : ucByte - Received byte from UART circular buffer
// Outputs : None
// Return  : None
// Notes   : Stores checksum byte and transitions to stop byte state
//******************************************************************************
static void UartFrameHandleChecksum(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;
    seRxState = UART_FRAME_RX_STOP;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Handle reception of stop byte and trigger frame processing
// Inputs  : ucByte - Received byte from UART circular buffer
// Outputs : None
// Return  : None
// Notes   : Validates stop byte processes complete frame, resets state machine
//******************************************************************************
static void UartFrameHandleStop(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (UART_STOP_BYTE == ucByte)
    {
        UartFrameProcessFull();
    }
    else
    {
        printf("[UartFrameReceiver] Bad STOP byte! Got 0x%02X\n", ucByte);
    }
    
    UartFrameResetState(); // Clear partial frame
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Process a complete received frame with validation and queuing
// Inputs  : None (uses global frame buffer and parsed header data)
// Outputs : None
// Return  : None
// Notes   : Validates frame integrity, allocates payload memory, queues frame for processing
//******************************************************************************
static void UartFrameProcessFull(void)
{
    DATA_FRAME sFrame = sPartialFrame;
    uint8* pucPayload = &sucFrameBuffer[1 + FRAME_HEADER_SIZE];
    uint8  ucChecksum = sucFrameBuffer[sunFrameIndex - 2];
    uint8  ucStop = sucFrameBuffer[sunFrameIndex - 1];
    sFrame.ucStartByte = UART_START_BYTE;
    sFrame.ucStopByte  = ucStop;
    sFrame.ucChecksum  = ucChecksum;

    uint32 ulExpectedLen =
        1 + FRAME_HEADER_SIZE + sFrame.ulLength + 1 + 1;

    if (ulExpectedLen == sunFrameIndex &&
        ParseValidateChecksum(pucPayload, sFrame.ulLength, ucChecksum) &&
        (ucStop == UART_STOP_BYTE) &&
        ParsePayload(&sFrame, pucPayload))
    {
        (void)UartFrameEnqueue(&sFrame);
    }
    else
    {
        printf("[UartFrameReceiver] Frame validation failed/discarded.\n");
    }

    if (sFrame.pucValue != NULL) 
    {
        free(sFrame.pucValue);
        sFrame.pucValue = NULL;
    }

}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Enqueue a validated frame to the protocol processing queue
// Inputs  : psFrame - Pointer to validated DATA_FRAME structure
// Outputs : None
// Return  : bool - TRUE if frame queued successfully, FALSE if queue full or error
// Notes   : Frees allocated payload memory on queue failure to prevent memory leaks
//******************************************************************************
static bool UartFrameEnqueue(DATA_FRAME *psFrame)
{
    bool blRet = false;

    if (psFrame != NULL)
    {
        if (OSQueueSend(gFrameQueueHandle, psFrame, 0))
        {
            blRet = true;
        }
        else
        {
            printf("[UartFrameReceiver] Failed to enqueue frame\n");
            if (psFrame->pucValue != NULL)
            {
                free(psFrame->pucValue);
                psFrame->pucValue = NULL;
            }
        }
    }
    return blRet;
}

//EOF
