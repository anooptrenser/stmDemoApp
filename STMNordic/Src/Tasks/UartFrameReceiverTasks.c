//******************************* UartFrameReceiver **************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartFrameReceiver.c
// Summary  : UART frame extraction thread implementation
// Author   : Anoop G
// Date     : 22-07-2025
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

//***************************** Global Variables ******************************
static uint8 s_frameBuffer[MAX_RAW_FRAME_LEN] = {0};
static uint16 s_frameIndex = 0;
static bool s_frameActive = false;

//************************ Static Function Prototypes *************************
static bool UartIsStartByte(uint8 byte);
static bool UartIsStopByte(uint8 byte);
static void UartStartNewFrame(void);
static void UartResetFrameState(void);
static void UartFrameBufferAppend(uint8 byte);
static void UartFrameProcess(void);

//******************************.FUNCTION_HEADER.******************************
// Purpose : Thread to extract frames from UART RX circular buffer and send to queue
// Inputs  : pvArgs - Not used
// Outputs : None
// Return  : None
// Notes   : Runs as RTOS task under FreeRTOS. Processes bytes continuously
//           using start/stop delimiters. Calls internal frame extraction logic.
//*****************************************************************************
void UartFrameReceiverTask(void *pvArgs)
{
    (void)pvArgs;
    uint8 receivedByte = 0;

    while (1)
    {
        if (!UartRxBufferPop(&gUartRxBuffer, &receivedByte)) 
        {
            OsTaskDelay(1);
            continue;
        }

        if (!s_frameActive)
        {
            if (UartIsStartByte(receivedByte)) 
            {
                UartStartNewFrame();
            }
            continue;
        }

        if (UartIsStopByte(receivedByte)) 
        {
            UartFrameProcess();
            UartResetFrameState();
            continue;
        }

        UartFrameBufferAppend(receivedByte);
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Checks whether a received byte is a start-of-frame marker.
// Inputs  : byte - The received byte from UART.
// Outputs : None
// Return  : true if byte == UART_START_BYTE (0xFF), else false.
// Notes   : Used to identify the beginning of a new frame.
//*****************************************************************************
static bool UartIsStartByte(uint8 byte)
{
    return (byte == UART_START_BYTE);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Checks whether a received byte is a stop-of-frame marker.
// Inputs  : byte - The received byte from UART.
// Outputs : None
// Return  : true if byte == UART_STOP_BYTE (0x0E), else false.
// Notes   : Used to identify the end of a frame to trigger frame decoding.
//*****************************************************************************
static bool UartIsStopByte(uint8 byte)
{
    return (byte == UART_STOP_BYTE);
}


//******************************.FUNCTION_HEADER.******************************
// Purpose : Initializes internal state for capturing new UART frame.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Called when a start byte is detected. Resets frame index and 
//           activates capture.
//*****************************************************************************
static void UartStartNewFrame(void)
{
    s_frameIndex = 0;
    s_frameActive = true;
    printf("[RX] Start byte detected, starting frame\n");
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Resets internal parser state to prepare for next frame.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Called after valid or invalid frame completion, or overflow.
//*****************************************************************************
static void UartResetFrameState(void)
{
    s_frameIndex = 0;
    s_frameActive = false;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Appends byte to current in-progress frame buffer.
// Inputs  : byte - One byte from UART data stream to store
// Outputs : None
// Return  : None
// Notes   : If buffer overflows, the frame is discarded and state resets.
//*****************************************************************************
static void UartFrameBufferAppend(uint8 byte)
{
    if (s_frameIndex < sizeof(s_frameBuffer)) 
    {
        s_frameBuffer[s_frameIndex++] = byte;
    } else 
    {
        printf("[ERROR] Frame buffer overflow — discarding frame\n");
        UartResetFrameState();
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Processes completed UART frame on detecting STOP byte.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Parses header, validates payload length and checksum, optionally
//           allocates memory and enqueues the frame to message queue.
//           Handles all frame consistency checks and errors internally.
//*****************************************************************************
static void UartFrameProcess(void)
{
    DATA_FRAME frame = {0};
    uint8* pucHeader = NULL;
    uint8* pucPayload = NULL;
    uint8 ucChecksum = 0;
    bool blValidFrame = false;

    // 1. Basic minimum length check
    if (s_frameIndex >= (FRAME_HEADER_SIZE + 1U)) // +1 for at least checksum
    {
        pucHeader = &s_frameBuffer[0];
        pucPayload = &s_frameBuffer[FRAME_HEADER_SIZE];
        ucChecksum = s_frameBuffer[s_frameIndex - 1];

        ParseHeader(pucHeader, &frame);
        frame.ucStartByte = UART_START_BYTE;
        frame.ucStopByte = UART_STOP_BYTE;
        frame.ucChecksum = ucChecksum;

        // 2. Length consistency check
        uint32_t expectedLen = frame.ulLength + FRAME_HEADER_SIZE + 1U; // +1 for checksum
        if (expectedLen == s_frameIndex)
        {
            // 3. Checksum validation
            if (ValidateChecksum(pucPayload, frame.ulLength, ucChecksum))
            {
                // 4. Payload allocation (if needed)
                if (frame.ulLength > 0U)
                {
                    frame.pucValue = malloc(frame.ulLength);
                    if (frame.pucValue != NULL)
                    {
                        memcpy(frame.pucValue, pucPayload, frame.ulLength);
                        blValidFrame = true;
                    }
                    else
                    {
                        printf("[ERROR] Memory allocation failed\n\r");
                    }
                }
                else
                {
                    frame.pucValue = NULL;
                    blValidFrame = true;
                }
            }
            else
            {
                printf("[ERROR] Checksum invalid\n");
            }
        }
        else
        {
            printf("[ERROR] Frame length mismatch (expected: %lu, actual: %u)\n",
                   frame.ulLength, s_frameIndex - FRAME_HEADER_SIZE - 1U);
        }
    }
    else
    {
        printf("[WARN] Frame too short — discarded\n");
    }

    if (blValidFrame)
    {
        if (!OSQueueSend(gFrameQueueHandle, &frame, 0))
        {
            printf("[ERROR] Failed to enqueue frame\n\r");
            if (frame.pucValue != NULL)
            {
                free(frame.pucValue);
                frame.pucValue = NULL;
            }
        }
        else
        {
            printf("\n\r[OK] Frame received: CMD=0x%02X TYPE=0x%02X LEN=%lu SEQ=%u\n\r",
                   frame.ucCmd, frame.ucType, frame.ulLength, frame.unSeqNum);
        }
    }
}

//EOF