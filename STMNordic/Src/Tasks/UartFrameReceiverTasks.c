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
#include <stdlib.h>

//***************************** Global Variables ******************************
static uint8 s_ucFrameBuffer[MAX_RAW_FRAME_LEN] = {0};
static uint16 s_unFrameIndex = 0;
static bool s_blFrameActive = false;

//************************ Static Function Prototypes *************************
static bool UartIsStartByte(uint8 ucByte);
static bool UartIsStopByte(uint8 ucByte);
static void UartStartNewFrame(void);
static void UartResetFrameState(void);
static void UartFrameBufferAppend(uint8 ucByte);
static bool UartFrameProcess(void);
static bool EnqueueFrame(DATA_FRAME* psFrame);

//******************************.FUNCTION_HEADER.******************************
// Purpose : UART frame receiver task reading bytes from UART RX circular buffer 
//           and extracting valid frames using start/stop delimiters.
// Inputs  : pvArgs - unused task parameter pointer
// Outputs : None
// Return  : None
// Notes   : Runs as an RTOS task continuously processing UART data stream.
//*****************************************************************************
void UartFrameReceiverTask(void *pvArgs)
{
    (void)pvArgs;
    uint8 ucReceivedByte = 0;

    while (1)
    {
        if (!UartRxBufferPop(&gUartRxBuffer, &ucReceivedByte))
        {
            OsTaskDelay(1);
            continue;
        }

        if (!s_blFrameActive)
        {
            if (UartIsStartByte(ucReceivedByte))
            {
                UartStartNewFrame();
            }
            continue;
        }

        if (UartIsStopByte(ucReceivedByte))
        {
            bool blProcessSuccess = UartFrameProcess(); 
             
            if (!blProcessSuccess)
            {
                printf("[ERROR] Frame processing failed\n");
            }

            UartResetFrameState();
            continue;
        }

        UartFrameBufferAppend(ucReceivedByte);
    }
}


//******************************.FUNCTION_HEADER.******************************
// Purpose : Check if the given byte is the UART start-of-frame byte.
// Inputs  : ucByte - The received byte from UART.
// Outputs : None
// Return  : true if equal to UART_START_BYTE (0xFF), false otherwise.
// Notes   : Used by frame receiver to detect start of a new frame.
//*****************************************************************************
static bool UartIsStartByte(uint8 ucByte)
{
    return (ucByte == UART_START_BYTE);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Check if the given byte is the UART stop-of-frame byte.
// Inputs  : ucByte - The received byte from UART.
// Outputs : None
// Return  : true if equal to UART_STOP_BYTE (0x0E), false otherwise.
// Notes   : Used by frame receiver to detect frame end.
//*****************************************************************************
static bool UartIsStopByte(uint8 ucByte)
{
    return (ucByte == UART_STOP_BYTE);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initialize/reset frame parsing buffer and state on start byte detection.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Called to begin capturing a new frame.
//*****************************************************************************
static void UartStartNewFrame(void)
{
    s_unFrameIndex = 0;
    s_blFrameActive = true;
    printf("[RX] Start byte detected, starting frame\n");
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Reset internal frame parsing state after frame completion or error.
// Inputs  : None
// Outputs : None
// Return  : None
// Notes   : Prepares task for next frame capture.
//*****************************************************************************
static void UartResetFrameState(void)
{
    s_unFrameIndex = 0;
    s_blFrameActive = false;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Append one byte to the current frame buffer being captured.
// Inputs  : ucByte - Byte to append from UART stream.
// Outputs : None
// Return  : None
// Notes   : Discards frame and resets state on buffer overflow.
//*****************************************************************************
static void UartFrameBufferAppend(uint8 ucByte)
{
    if (s_unFrameIndex < sizeof(s_ucFrameBuffer)) 
    {
        s_ucFrameBuffer[s_unFrameIndex++] = ucByte;
    }
    else 
    {
        printf("[ERROR] Frame buffer overflow — discarding frame\n");
        UartResetFrameState();
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Send parsed and validated frame to the frame queue for processing.
// Inputs  : psFrame - Pointer to the DATA_FRAME struct to enqueue.
// Outputs : None
// Return  : true if successfully enqueued, false otherwise.
// Notes   : On enqueue failure, frees allocated payload memory to avoid leaks.
//*****************************************************************************
static bool EnqueueFrame(DATA_FRAME *psFrame)
{
    bool blRet = false;  

    if (psFrame != NULL)
    {
        if (OSQueueSend(gFrameQueueHandle, psFrame, 0))
        {
            printf("\n\r[OK] Frame received: CMD=0x%02X TYPE=0x%02X LEN=%lu SEQ=%u\n\r",
                   psFrame->ucCmd, psFrame->ucType, psFrame->ulLength, psFrame->unSeqNum);
            blRet = true;
        }
        else
        {
            printf("[ERROR] Failed to enqueue frame\n\r");
            if (psFrame->pucValue != NULL)
            {
                free(psFrame->pucValue);
                psFrame->pucValue = NULL;
            }
        }
    }

    return blRet;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Processes a complete UART frame on detection of the stop byte.
//           Validates header, length, checksum; copies payload; enqueues frame.
// Inputs  : None (uses internal static frame buffer variables)
// Outputs : None
// Return  : true if frame processed and enqueued successfully, false otherwise
// Notes   : Handles all validation, memory management, and error reporting internally.
//*****************************************************************************
static bool UartFrameProcess(void)
{
    DATA_FRAME sFrame = {0};
    uint8 *pucHeader = NULL;
    uint8 *pucPayload = NULL;
    uint8 ucChecksum = 0;
    uint32 ulExpectedLen = 0;
    bool blValidFrame = false;
    bool blRet = false;

    if (s_unFrameIndex < (FRAME_HEADER_SIZE + 1U))
    {
        printf("[WARN] Frame too short — discarded\n");
        blRet = false; 
    }
    else
    {
        pucHeader  = &s_ucFrameBuffer[0];
        pucPayload = &s_ucFrameBuffer[FRAME_HEADER_SIZE];
        ucChecksum = s_ucFrameBuffer[s_unFrameIndex - 1];

        ParseHeader(pucHeader, &sFrame);
        sFrame.ucStartByte = UART_START_BYTE;
        sFrame.ucStopByte  = UART_STOP_BYTE;
        sFrame.ucChecksum  = ucChecksum;

        ulExpectedLen = sFrame.ulLength + FRAME_HEADER_SIZE + 1U;

        if (ulExpectedLen != s_unFrameIndex)
        {
            printf("[ERROR] Frame length mismatch (expected: %lu, actual: %u)\n",
                   sFrame.ulLength, s_unFrameIndex - FRAME_HEADER_SIZE - 1U);
            blRet = false;
        }
        else if (!ParseValidateChecksum(pucPayload, sFrame.ulLength, ucChecksum))
        {
            printf("[ERROR] Checksum invalid\n");
            blRet = false;
        }
        else if (!ParsePayload(&sFrame, pucPayload))
        {
            blRet = false;
        }
        else
        {
            blValidFrame = true;
            blRet = true;  // All validations passed
        }
    }

    if (blValidFrame)
    {
        blRet = EnqueueFrame(&sFrame);
    }

    return blRet;
}

// EOF
