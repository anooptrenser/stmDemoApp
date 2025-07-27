//******************************* FileTransferManager *************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferManager.c
// Summary  : High-level file transfer task implementation (with chunk ACK)
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
//************************* Include Files *************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "FileTransferManager.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "UartDriver.h"
//******************************* Local Types ********************************* 

//***************************** Local Constants ******************************* 

//***************************** Local Variables ******************************* 

//****************************** Local Functions ******************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk);
static bool FileTransfer(const uint8* pucData, const uint32 ulLen, uint32 ulMaxChunk);
static bool FileTransferComplete(void);
static bool FileTransferAckWait(uint16 unExpectedSeqNum, uint32 ulTimeoutMs);
static bool FileTransferFrameFromQueue(DATA_FRAME* psFrame, uint32 ulTimeoutMs);
static bool FileTransferAckWaitWithRetry(const uint16 unSeqNum);

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for ACK for a given sequence number from receiver
// Inputs   : unExpectedSeqNum - Expected ACK sequence number
//          : ulTimeoutMs      - Timeout for ACK wait
// Outputs  : None
// Returns  : bool             - TRUE if correct ACK received, FALSE otherwise
//*****************************************************************************
static bool FileTransferAckWait(uint16 unExpectedSeqNum, uint32 ulTimeoutMs)
{
    DATA_FRAME stAck = {0};
    bool blStatus = false;

    blStatus = FileTransferFrameFromQueue(&stAck, ulTimeoutMs);

    if (blStatus == true)
    {
        if ((stAck.ucCmd == CMD_TRANSFER) &&
            (stAck.ucType == TYPE_ACK) &&
            (stAck.unSeqNum == unExpectedSeqNum) &&
            (stAck.ulLength == 0U))
        {
            blStatus = true;
        }
        else
        {
            blStatus = false;
        }

        if (stAck.pucValue != NULL)
        {
            free(stAck.pucValue);
            stAck.pucValue = NULL;
        }
    }

return blStatus;

}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for a frame from the protocol queue with timeout
// Inputs   : psFrame      - Pointer to DATA_FRAME structure to fill
//          : ulTimeoutMs  - Timeout in milliseconds to wait for frame
// Outputs  : None
// Return   : bool         - TRUE if frame received, FALSE if timeout or error
// Notes    : Uses OSQueueRecv to receive from gFrameQueueHandle
//*****************************************************************************
static bool FileTransferFrameFromQueue(DATA_FRAME* psFrame, uint32 ulTimeoutMs)
{
    return OSQueueRecv(gFrameQueueHandle, psFrame, ulTimeoutMs);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for ACK with retry mechanism for a given sequence number
// Inputs   : unSeqNum - Sequence number to wait for ACK
// Outputs  : None
// Returns  : bool     - TRUE if ACK received within retries, FALSE otherwise
//*****************************************************************************
static bool FileTransferAckWaitWithRetry(const uint16 unSeqNum)
{
    bool blAckReceived = false;

    for (int nRetry = 0; nRetry < MAX_ACK_RETRIES; ++nRetry)
    {
        if (FileTransferAckWait(unSeqNum, DATA_SENDER_TIMEOUT_MS))
        {
            blAckReceived = true;
            break;
        } else {
            printf("Warning: ACK not received for seq %u (retry %d/%d)\r\n",
                   unSeqNum, nRetry + 1, MAX_ACK_RETRIES);
        }
    }

    if (!blAckReceived)
    {
        printf("Error: Retries exceeded for seq %u\r\n", unSeqNum);
    }

    return blAckReceived;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Application entry for the protocol file transfer task.
// Inputs   : pucData   - Pointer to the data buffer
//          : ulFileLen - Length of the data buffer
// Outputs  : None
// Returns  : bool      - TRUE if transfer complete, FALSE if failed.
//*****************************************************************************
bool FileTransferManager(const uint8* pucData, uint32 ulFileLen)
{
	uint32 ulMaxChunk = 0U;
    bool blStatus = false;

    blStatus = InitFileTransfer(ulFileLen, &ulMaxChunk);

    if (blStatus != true)
    {
        printf("Error: InitFileTransfer failed\r\n");
    }
    else
    {
        blStatus = FileTransfer(pucData, ulFileLen, ulMaxChunk);
        
        if (blStatus != true)
        {
            printf("Error: FileTransfer failed\r\n");
        }
        else
        {
            // Send transfer complete notification
            if (FileTransferComplete())
            {
                printf("File transfer completed successfully\r\n");
            }
            else
            {
                printf("Warning: Transfer complete notification failed\r\n");             
            }
        }
    }

    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Send file length and get max chunk size from receiver.
// Inputs   : ulFileLen   - Length of file in bytes.
//          : pulMaxChunk - Pointer to store max chunk size.
// Outputs  : None
// Returns  : bool        - TRUE if handshake ok, FALSE if error.
//*****************************************************************************
static bool InitFileTransfer(uint32 ulFileLen, uint32 *pulMaxChunk)
{
    DATA_FRAME stInit = {0};
    DATA_FRAME stResp = {0};
    uint8 ucFileLen[FILE_LEN_BYTES] = {0};
    bool blSendResult = false;
    bool blRecvResult = false;
    bool blValidResp = false;

    if (pulMaxChunk != NULL)
    {
        memcpy(ucFileLen, &ulFileLen, FILE_LEN_BYTES);

        stInit.ucCmd = CMD_INIT;
        stInit.ucType = TYPE_FILE_LENGTH;
        stInit.ulLength = FILE_LEN_BYTES;
        stInit.unSeqNum = SEQ_INIT;
        stInit.pucValue = ucFileLen;
        stInit.ucChecksum = UartProtoCalcChecksum(ucFileLen, stInit.ulLength);

        blSendResult = UartProtoSendFrame(&stInit);

        blRecvResult = FileTransferFrameFromQueue(&stResp, DATA_SENDER_TIMEOUT_MS);

        if (blRecvResult == true)
        {
            blValidResp = ((stResp.ucCmd == CMD_INIT) &&
                           (stResp.ucType == TYPE_CHUNK_SIZE) &&
                           (stResp.ulLength == FILE_LEN_BYTES)) ? true : false;
        
            if (blValidResp == true)
            {
                memcpy(pulMaxChunk, stResp.pucValue, FILE_LEN_BYTES);
            }
            // Free buffer here regardless
            if (stResp.pucValue != NULL)
            {
                free(stResp.pucValue);
                stResp.pucValue = NULL;
            }
        }

    }

    return blSendResult;
}


//******************************.FUNCTION_HEADER.******************************
// Purpose  : Send file data in multiple frames/chunks, handling ACK per chunk
// Inputs   : pucData    - File buffer to send
//          : ulLen      - Total file length in bytes
//          : ulMaxChunk - Max chunk size allowed by receiver
// Outputs  : None
// Returns  : bool       - TRUE when the full file is sent
//*****************************************************************************
static bool FileTransfer(const uint8* pucData, const uint32 ulLen, const uint32 ulMaxChunk)
{
    bool blStatus = false;
    uint32 ulOffset = 0;
    uint16 unSeqNum = 1;
    uint8 ucChunkBuf[MAX_FRAME_SIZE] = {0};
    uint32 ulChunkLen = 0;

    if ((pucData != NULL) && (ulLen > 0U) && (ulMaxChunk > 0U))
    {
        while (ulOffset < ulLen)
        {
            ulChunkLen = ((ulLen - ulOffset) > ulMaxChunk) ? ulMaxChunk : (ulLen - ulOffset);

            if (ulChunkLen > sizeof(ucChunkBuf))
            {
                printf("Error: Chunk size too large\r\n");
                break;
            }

            memcpy(ucChunkBuf, &pucData[ulOffset], ulChunkLen);

            if (!UartProtoSendChunk(unSeqNum, ucChunkBuf, ulChunkLen))
            {
                break;
            }

            if (!FileTransferAckWaitWithRetry(unSeqNum))
            {
                break;
            }

            ulOffset += ulChunkLen;
            unSeqNum++;

            // Handle sequence number rollover (1 to 0xFFFF, skip 0)
            if (unSeqNum == 0) 
            {
                unSeqNum = 1;
            }
        }

        if (ulOffset >= ulLen)
        {
            blStatus = true;
        }
    }

    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Send transfer complete notification to receiver
// Inputs   : None
// Outputs  : None
// Returns  : bool - TRUE if notification sent successfully, FALSE otherwise
// Notes    : Sends a TYPE_COMPLETE frame to signal end of transfer
//*****************************************************************************
static bool FileTransferComplete(void)
{
    DATA_FRAME stComplete = {0};
    bool blSendResult = false;

    stComplete.ucCmd = CMD_TRANSFER;
    stComplete.ucType = TYPE_COMPLETE;
    stComplete.ulLength = 0U;  // No payload needed
    stComplete.unSeqNum = SEQ_COMPLETE;
    stComplete.pucValue = NULL;
    stComplete.ucChecksum = UartProtoCalcChecksum(NULL, 0U);

    blSendResult = UartProtoSendFrame(&stComplete);
    
    if (blSendResult)
    {
        printf("Transfer complete notification sent\r\n");
    }
    else
    {
        printf("Failed to send transfer complete notification\r\n");
    }

    return blSendResult;
}

// EOF
