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
#include "FrameBuilder.h"
#include "FrameParser.h"
#include "UartDriver.h"

//************************* Local Function Prototypes *************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk);
static bool FileTransfer(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk);
static bool WaitForAck(uint32 ulExpectedSeqNum, uint32 ulTimeoutMs);

//*****************************************************************************
// Function : FileTransferManager
// Purpose  : Application entry for the protocol file transfer task.
// Inputs   : pucData - Pointer to the data buffer
//			: ulFileLen - Length of the data buffer
// Outputs  : None
// Returns  : bool   - TRUE if transfer complete, FALSE if failed.
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

//*****************************************************************************
// Function : InitFileTransfer
// Purpose  : Send file length and get max chunk size from receiver.
// Inputs   : ulFileLen     - Length of file in bytes.
//            pulMaxChunk   - Pointer to store max chunk size.
// Outputs  : None
// Returns  : bool          - TRUE if handshake ok, FALSE if error.
//*****************************************************************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk)
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
        stInit.ulSeqNum = SEQ_INIT;
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

//*****************************************************************************
// Function : WaitForAck
// Purpose  : Waits for ACK for a given sequence number from receiver
// Inputs   : ulExpectedSeqNum - Expected ACK sequence number
//            ulTimeoutMs      - Timeout for ACK wait
// Outputs  : None
// Returns  : bool              - TRUE if correct ACK received, FALSE otherwise
//*****************************************************************************
static bool WaitForAck(uint32 ulExpectedSeqNum, uint32 ulTimeoutMs)
{
    DATA_FRAME stAck = {0};
    uint8 ackBuf[1] = {0}; // Length 0 payload for ACK
    bool blStatus = false;

    blStatus = ReceiveDataFrame(&stAck, ackBuf, sizeof(ackBuf), ulTimeoutMs);

    if (blStatus == true)
    {
        if ((stAck.ucCmd == CMD_TRANSFER) &&
            (stAck.ucType == TYPE_ACK) &&
            (stAck.ulSeqNum == ulExpectedSeqNum) &&
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

//*****************************************************************************
// Function : FileTransfer
// Purpose  : Send file data in multiple frames/chunks, handling ACK per chunk
// Inputs   : pucData    - File buffer to send
//            ulLen      - Total file length in bytes
//            ulMaxChunk - Max chunk size allowed by receiver
// Outputs  : None
// Returns  : bool       - TRUE when the full file is sent
//*****************************************************************************
static bool FileTransfer(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk)
{
    bool blStatus = false;
    bool blSendResult = false;
    uint32 ulSeqNum = 1U;
    uint32 ulOffset = 0U;
    uint8 ucChunkBuf[MAX_FRAME_SIZE] = {0};
    uint32 ulChunk = 0U;
    DATA_FRAME stData = {0};

    if ((pucData != NULL) && (ulLen > 0U) && (ulMaxChunk > 0U))
    {
        while (ulOffset < ulLen)
        {
            ulChunk = ((ulLen - ulOffset) > ulMaxChunk) ? ulMaxChunk : (ulLen - ulOffset);

            if (ulChunk > sizeof(ucChunkBuf))
            {
                printf("Error: ulChunk size exceeds ucChunkBuf\r\n");
                blStatus = false;
                break;
            }

            memcpy(ucChunkBuf, &pucData[ulOffset], ulChunk);

            stData.ucCmd = CMD_TRANSFER;
            stData.ucType = TYPE_DATA;
            stData.ulLength = ulChunk;
            stData.ulSeqNum = ulSeqNum;
            stData.pucValue = ucChunkBuf;
            stData.ucChecksum = CalcChecksum(ucChunkBuf, ulChunk);

            blSendResult = SendDataFrame(&stData);

            if (blSendResult != true)
            {
                printf("Error: SendFrame failed at chunk %lu\r\n", (unsigned long)ulSeqNum);
                blStatus = false;
                break;
            }

            // Wait for ACK from receiver for this chunk
            if (WaitForAck(ulSeqNum, DATA_SENDER_TIMEOUT_MS) != true)
            {
                printf("Error: No valid ACK for chunk %lu\r\n", (unsigned long)ulSeqNum);
                blStatus = false;
                break;
            }

            ulOffset += ulChunk;
            ulSeqNum++;
        }

        if (ulOffset >= ulLen)
        {
            blStatus = true;
        }
    }

    return blStatus;
}

// EOF
