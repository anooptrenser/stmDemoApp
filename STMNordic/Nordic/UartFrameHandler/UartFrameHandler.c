//*************************** UartFrameReceiver *******************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartFrameHandler.c
// Summary  : UART frame extraction 
// Author   : Anoop G 
// Date     : 27-07-2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UartFrameHandler.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "CircularBuffer.h"
#include "common.h"
#include "AppMain.h"
#include "Tmp.h"

//******************************* Local Types ********************************* 

//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 
static uint8  sucFrameBuffer[MAX_RAW_FRAME_LEN] = {0};
static uint16 sunFrameIndex = 0;
static uint32 sulPayloadLen = 0;
static DATA_FRAME sParsedFrame = {0};
static uint32 sulExpectedFileSize = 0;
static uint32 sulReceivedBytes = 0;
static bool blFileTransferInProgress = false;
static UART_FRAME_RX_STATE seRxState = UART_FRAME_RX_WAIT_START;

//************************ Static Function Prototypes *************************
static void UartFrameHandleWaitStart(uint8 ucByte);
static void UartFrameHandleHeader(uint8 ucByte);
static void UartFrameHandlePayload(uint8 ucByte);
static void UartFrameHandleChecksum(uint8 ucByte);
static void UartFrameHandleStop(uint8 ucByte);
static void UartFrameProcessFull(void);
static void UartFrameResetState(void);
static void UartFrameProcessReceived(DATA_FRAME *psFrame);
static void UartFrameHandleInit(const DATA_FRAME *psFrame);
static void UartFrameHandleTransfer(const DATA_FRAME *psFrame);
static void UartFrameHandleFileTransferComplete(void);

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Polls and processes UART frames using a state machine.
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : This function continuously checks the UART receive buffer for
//            incoming bytes. Each byte is then processed by a state machine
//            to reconstruct complete UART frames. It handles different states
//            of frame reception (wait for start, header, payload, checksum,
//            and stop byte) and includes a defensive mechanism to reset the
//            state machine in case of a buffer overflow or corrupted frame.
//*****************************************************************************
void UartFrameReceiverLoop(void)
{
    uint8 ucReceivedByte = 0;
    
    while (UartRxBufferCount(&gUartRxBuffer) > 0)
    {
        UartRxBufferPop(&gUartRxBuffer, &ucReceivedByte);

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

        // Defensive: buffer overflow resets state (corrupt/invalid frame recovery)
        if (sunFrameIndex >= MAX_RAW_FRAME_LEN)
        {
            printf("[UartFrameReceiver] Buffer overflow, resetting\n");
            UartFrameResetState();
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle byte received in WAIT_START state - look for start byte
// Inputs   : ucByte    - Received byte to process
// Outputs  : Updates state machine and frame buffer
// Returns  : None
//*****************************************************************************
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
// Purpose  : Handle byte received in HEADER state - collect header bytes
// Inputs   : ucByte    - Received byte to process
// Outputs  : Updates state machine, parses header when complete
// Returns  : None
//*****************************************************************************
static void UartFrameHandleHeader(uint8 ucByte)
{
    uint8* pucHeader = NULL;
    
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (sunFrameIndex == (FRAME_HEADER_SIZE + 1))
    {
        pucHeader = &sucFrameBuffer[1];
        ParseHeader(pucHeader, &sParsedFrame);  // Parse once and store
        sulPayloadLen = sParsedFrame.ulLength;

        if (sulPayloadLen > MAX_FRAME_SIZE) 
        {
            printf("[UartFrameReceiver] Payload too large, discarding.\n");
            UartFrameResetState();
        } else 
        {
            seRxState = (sulPayloadLen > 0) ?
                        UART_FRAME_RX_PAYLOAD : UART_FRAME_RX_CHECKSUM;
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle byte received in PAYLOAD state - collect payload data
// Inputs   : ucByte    - Received byte to process
// Outputs  : Updates state machine when payload collection complete
// Returns  : None
//*****************************************************************************
static void UartFrameHandlePayload(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (sunFrameIndex == (FRAME_HEADER_SIZE + 1 + sulPayloadLen)) 
    {
        seRxState = UART_FRAME_RX_CHECKSUM;
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle byte received in CHECKSUM state - collect checksum byte
// Inputs   : ucByte    - Received byte to process
// Outputs  : Updates state machine to expect stop byte
// Returns  : None
//*****************************************************************************
static void UartFrameHandleChecksum(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;
    seRxState = UART_FRAME_RX_STOP;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle byte received in STOP state - validate and process frame
// Inputs   : ucByte    - Received byte to process
// Outputs  : Processes complete frame or reports error
// Returns  : None
//*****************************************************************************
static void UartFrameHandleStop(uint8 ucByte)
{
    sucFrameBuffer[sunFrameIndex++] = ucByte;

    if (UART_STOP_BYTE == ucByte)
    {
        UartFrameProcessFull();

    } else
    {
        printf("[UartFrameReceiver] Bad STOP byte! Got 0x%02X\n", ucByte);
    }

    UartFrameResetState();
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
    memset(&sParsedFrame, 0, sizeof(sParsedFrame));
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Process complete frame using already parsed header data
// Inputs   : None (uses static frame buffer and parsed header)
// Outputs  : Validates frame and dispatches for command processing
// Returns  : None
//*****************************************************************************
static void UartFrameProcessFull(void)
{
    DATA_FRAME sFrame = {0};
    uint8* pucPayload = NULL;
    uint8  ucChecksum = 0;
    uint8  ucStop = 0;
    uint32 ulExpectedLen = 0;
    
    // Display received frame data using HexDump for debugging
    printf("[UartFrameReceiver] Raw frame data:\n\r");
    HexDump(sucFrameBuffer, sunFrameIndex);

    sFrame = sParsedFrame;
    pucPayload = &sucFrameBuffer[1 + FRAME_HEADER_SIZE];
    ucChecksum = sucFrameBuffer[sunFrameIndex - 2];
    ucStop = sucFrameBuffer[sunFrameIndex - 1];
    sFrame.ucStartByte = UART_START_BYTE;
    sFrame.ucStopByte  = ucStop;
    sFrame.ucChecksum  = ucChecksum;

    ulExpectedLen = 1 + FRAME_HEADER_SIZE + sFrame.ulLength + 1 + 1;

    if (ulExpectedLen == sunFrameIndex &&
        ParseValidateChecksum(pucPayload, sFrame.ulLength, ucChecksum) &&
        (ucStop == UART_STOP_BYTE) &&
        ParsePayload(&sFrame, pucPayload))
    {
        UartFrameProcessReceived(&sFrame);
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
// Purpose  : Dispatch received frame to appropriate command handler
// Inputs   : psFrame   - Pointer to validated frame data structure
// Outputs  : Routes frame to specific command/type handlers
// Returns  : None
//*****************************************************************************
static void UartFrameProcessReceived(DATA_FRAME *psFrame)
{
    switch(psFrame->ucCmd)
    {
        case CMD_INIT:
        {
            if (psFrame->ucType == TYPE_FILE_LENGTH)
            {
                UartFrameHandleInit(psFrame);
            }
            else
            {
                printf("[UartFrameReceiver] CMD_INIT received, unknown type: 0x%02X\n\r", 
                        psFrame->ucType);
            }

            break;
        }
            
        case CMD_TRANSFER:
        {
            if (psFrame->ucType == TYPE_DATA)
            {
                UartFrameHandleTransfer(psFrame);
            }
            else if (psFrame->ucType == TYPE_COMPLETE)
            {
                UartFrameHandleFileTransferComplete();
            }
            else
            {
                printf("[UartFrameReceiver] CMD_TRANSFER unknown type: 0x%02X\n\r", 
                        psFrame->ucType);
            }

            break;
        }
            
        default:
        {
            printf("[UartFrameReceiver] Unknown command. Cmd=0x%02X, Type=0x%02X\n\r",
                    psFrame->ucCmd, psFrame->ucType);
            break;
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle CMD_INIT frame with file transfer initialization
// Inputs   : psFrame   - Pointer to frame containing file size information
// Outputs  : Initializes transfer state, sends chunk size response
// Returns  : None
//*****************************************************************************
static void UartFrameHandleInit(const DATA_FRAME *psFrame)
{
    uint32 ulMaxChunkSize = 128U;
    DATA_FRAME stResp = {0};
    
    if (psFrame != NULL && psFrame->pucValue != NULL && psFrame->ulLength == sizeof(uint32))
    {
        // Extract file size from the received data
        sulExpectedFileSize = *((uint32*)psFrame->pucValue);
        sulReceivedBytes = 0;
        blFileTransferInProgress = true;

        printf("[UartFrameReceiver] File transfer initiated. Expected size: %lu bytes\n", 
                sulExpectedFileSize);
    }
    
    stResp.ucCmd = CMD_INIT;
    stResp.ucType = TYPE_CHUNK_SIZE;
    stResp.ulLength = sizeof(ulMaxChunkSize);
    stResp.unSeqNum = 0;
    stResp.pucValue = (uint8*)&ulMaxChunkSize;
    stResp.ucChecksum = UartProtoCalcChecksum(stResp.pucValue, stResp.ulLength);

    if (!UartProtoSendFrame(&stResp))
    {
        printf("[UartFrameReceiver] Error: SendDataFrame failed for CMD_INIT response\n");
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle CMD_TRANSFER frame with data chunk
// Inputs   : psFrame   - Pointer to frame containing data chunk
// Outputs  : Updates transfer progress, sends ACK response
// Returns  : None
//*****************************************************************************
static void UartFrameHandleTransfer(const DATA_FRAME *psFrame)
{
    DATA_FRAME stAck = {0};
    
    if (psFrame != NULL)
    {
        // Update received bytes count if file transfer is in progress
        if (blFileTransferInProgress && psFrame->pucValue != NULL)
        {
            sulReceivedBytes += psFrame->ulLength;
            printf("[UartFrameReceiver] Received chunk: %lu bytes, Total: %lu/%lu bytes\n", 
                   psFrame->ulLength, sulReceivedBytes, sulExpectedFileSize);
            
        }
        
        stAck.ucCmd = CMD_TRANSFER;
        stAck.ucType = TYPE_ACK;
        stAck.ulLength = 0;
        stAck.unSeqNum = psFrame->unSeqNum;
        stAck.pucValue = NULL;
        stAck.ucChecksum = 0;

        if (!UartProtoSendFrame(&stAck))
        {
            printf("[UartFrameReceiver] Error: SendDataFrame failed for ACK\n");
        }
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Handle file transfer completion notification from STM32
// Inputs   : None (uses static transfer state variables)
// Outputs  : Validates transfer, resets state, sends final ACK
// Returns  : None
//*****************************************************************************
static void UartFrameHandleFileTransferComplete(void)
{
    DATA_FRAME stFinalAck = {0};
    blFileTransferInProgress = false;
    sulExpectedFileSize = 0;
    sulReceivedBytes = 0;
    
    printf("[UartFrameReceiver] *** FILE TRANSFER COMPLETE ***\n");
    printf("[UartFrameReceiver] Total bytes received: %lu\n", sulReceivedBytes);
    printf("[UartFrameReceiver] Expected bytes: %lu\n", sulExpectedFileSize);
    
    // Validate that we received the expected amount of data
    if (sulReceivedBytes == sulExpectedFileSize)
    {
        printf("[UartFrameReceiver] File transfer validation: SUCCESS\n");
    }
    else
    {
        printf("[UartFrameReceiver] File transfer validation: MISMATCH (Expected: %lu, Received: %lu)\n", 
                sulExpectedFileSize, sulReceivedBytes);
    }
      
}
// EOF
