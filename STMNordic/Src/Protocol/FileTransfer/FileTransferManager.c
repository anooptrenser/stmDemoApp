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
#include <stdlib.h>
#include "FileTransferManager.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "UartDriver.h"
#include "FileTransferHelper.h"

//************************* Local Function Prototypes *************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk);
static bool FileTransfer(const uint8* pucData, const uint32 ulLen, uint32 ulMaxChunk);

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

        blRecvResult = WaitForFrameFromQueue(&stResp, DATA_SENDER_TIMEOUT_MS);

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

            if (!WaitForAckWithRetry(unSeqNum))
            {
                break;
            }

            ulOffset += ulChunkLen;
            unSeqNum++;

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

// EOF
