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
//*********************Include Files*******************************************
#include <string.h>
#include <stdio.h>
#include "FileTransferManager.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "UartDriver.h"

//************************* Local Function Prototypes *************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk);
static bool FileTransfer(const uint8* pucData, const uint32 ulLen, uint32 ulMaxChunk);
static bool WaitForAck(uint16 unExpectedSeqNum, uint32 ulTimeoutMs);
static bool WaitForAckWithRetry(const uint16 unSeqNum);
static bool SendChunk(const uint16 unSeqNum, const uint8* pucData, const uint32 ulLength);

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Entry point for initiating a file transfer using frame protocol.
// Inputs   : pucData   - Pointer to the file/data buffer to be transferred.
//            ulFileLen - Size of the data buffer in bytes.
// Outputs  : None
// Returns  : bool      - TRUE if transfer is successful, FALSE otherwise.
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
    }

    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Performs protocol initialization by transmitting file length
//            and receiving receiver's maximum allowed chunk size.
// Inputs   : ulFileLen   - Length of the file to transfer (in bytes).
//            pulMaxChunk - Pointer to buffer to receive max chunk size.
// Outputs  : pulMaxChunk - Filled with chunk size supported by receiver.
// Returns  : bool        - TRUE if handshake successful, FALSE on failure.
//*****************************************************************************
static bool InitFileTransfer(uint32 ulFileLen, uint32 *pulMaxChunk)
{
    DATA_FRAME stInit = {0};
    DATA_FRAME stResp = {0};
    uint8 ucFileLen[FILE_LEN_BYTES] = {0};
    uint8 ucRespVal[FILE_LEN_BYTES] = {0};
    bool blStatus = false;
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
        stInit.ucChecksum = CalcChecksum(ucFileLen, stInit.ulLength);

        blSendResult = SendDataFrame(&stInit);

        if (blSendResult == true)
        {
            blRecvResult = ReceiveDataFrame(&stResp, ucRespVal, FILE_LEN_BYTES,
            								DATA_SENDER_TIMEOUT_MS);

            if (blRecvResult == true)
            {
                blValidResp = ((stResp.ucCmd == CMD_INIT) &&
                               (stResp.ucType == TYPE_CHUNK_SIZE) &&
                               (stResp.ulLength == FILE_LEN_BYTES)) ? true : false;

                if (blValidResp == true)
                {
                    memcpy(pulMaxChunk, ucRespVal, FILE_LEN_BYTES);
                    blStatus = true;
                }
            }
        }
    }

    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for an acknowledgment frame (TYPE_ACK) for given sequence.
// Inputs   : unExpectedSeqNum - Sequence number to match in received ACK.
//            ulTimeoutMs      - UART receive timeout in milliseconds.
// Outputs  : None
// Returns  : bool              - TRUE if correct ACK received, FALSE otherwise.
//*****************************************************************************
static bool WaitForAck(uint16 unExpectedSeqNum, uint32 ulTimeoutMs)
{
    DATA_FRAME stAck = {0};
    uint8 ackBuf[1] = {0}; 
    bool blStatus = false;

    blStatus = ReceiveDataFrame(&stAck, ackBuf, sizeof(ackBuf), ulTimeoutMs);

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
    }
    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Builds and transmits a single data chunk frame.
// Inputs   : unSeqNum  - Frame sequence number.
//            pucData   - Pointer to chunk data.
//            ulLength  - Length of data in bytes.
// Outputs  : None
// Returns  : bool      - TRUE if frame successfully sent, FALSE otherwise.
//*****************************************************************************
static bool SendChunk(const uint16 unSeqNum, const uint8* pucData, const uint32 ulLength)
{
    DATA_FRAME stData = {0};
    uint8 ucChecksum = CalcChecksum(pucData, ulLength);

    stData.ucCmd = CMD_TRANSFER;
    stData.ucType = TYPE_DATA;
    stData.ulLength = ulLength;
    stData.unSeqNum = unSeqNum;
    stData.pucValue = (uint8*)pucData;
    stData.ucChecksum = ucChecksum;

    if (!SendDataFrame(&stData)) {
        printf("Error: SendDataFrame failed at seq %u\r\n", unSeqNum);
        return false;
    }

    return true;
}

//******************************.FUNCTION_HEADER.******************************
// Function : WaitForAckWithRetry
// Purpose  : Wrapper around WaitForAck that retries up to MAX_ACK_RETRIES times.
// Inputs   : unSeqNum - Sequence number expected in ACK frame.
// Outputs  : None
// Returns  : bool      - TRUE if ACK received within retries, FALSE otherwise.
//*****************************************************************************
static bool WaitForAckWithRetry(const uint16 unSeqNum)
{
    bool blAckReceived = false;

    for (int8 cRetry = 0; cRetry < MAX_ACK_RETRIES; ++cRetry)
    {
        if (WaitForAck(unSeqNum, DATA_SENDER_TIMEOUT_MS))
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
// Purpose  : Transmits the entire buffer in multiple framed chunks
//            and waits for ACK after each chunk.
// Inputs   : pucData    - Pointer to complete file/buffer to transfer.
//            ulLen      - Total file size in bytes.
//            ulMaxChunk - Max chunk length supported by receiver.
// Outputs  : None
// Returns  : bool       - TRUE if successful transfer, FALSE on any failure.
// Notes    : Starts sequence number at 1. Rolls over 0xFFFF → 1 (wrap around).
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

            if (!SendChunk(unSeqNum, ucChunkBuf, ulChunkLen))
            {
                break;
            }

            if (!WaitForAckWithRetry(unSeqNum))
            {
                break;
            }

            ulOffset += ulChunkLen;
            unSeqNum++;

            if (unSeqNum == 0) {
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

// EOF
