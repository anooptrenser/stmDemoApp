//******************************* DataSender ***********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
// *****************************************************************************
//
// File     : DataSender.c
// Summary  : Implements TLV protocol sender for UART file transfer with sequence number
// Note     : None
// Author   : Anoop G
// Date     : 14-07-2025
//
// *****************************************************************************

//********************* Include Files *******************************************
#include "DataSender.h"
#include "DataFrame.h"
#include "UartDriver.h"
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "tmp.h"
#include <stdio.h>
#include <stddef.h>

//********************* Local Function Declarations *****************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk);
static bool FileTransfer(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk);

//********************* .DataSender *********************************************
// Purpose : Top-level function to perform TLV file transfer over UART
// Inputs  : None
// Outputs : TRUE on success, FALSE on error
// Return  : bool
// Notes   : Application entry point for sender
// *****************************************************************************
bool DataSender(void)
{
    uint32 ulMaxChunk = 0U;
    bool blStatus = false;

    // Step 1: Announce file length and get max chunk size from receiver
    blStatus = InitFileTransfer(g_ulJsonLen, &ulMaxChunk);

    if (!blStatus)
    {
        printf("Error: InitFileTransfer failed\r\n");
    }
    else
    {
        // Step 2: Send file data in chunks
        blStatus = FileTransfer(g_pucJson, g_ulJsonLen, ulMaxChunk);

        if (!blStatus)
        {
            printf("Error: FileTransfer failed\r\n");
        }
    }
    return blStatus;
}

//********************* .InitFileTransfer **************************************
// Purpose : Send file length to receiver and get max chunk size
// Inputs  : ulFileLen   - Length of file/data to send
//           pulMaxChunk - Pointer to store max chunk size from receiver
// Outputs : TRUE on success, FALSE on error
// Notes   : Handles all error conditions and returns only at end
// *****************************************************************************
static bool InitFileTransfer(uint32 ulFileLen, uint32* pulMaxChunk)
{
    DATA_FRAME stAnnounce = {0};
    DATA_FRAME stResp = {0};
    uint8 ucFileLen[FILE_LEN_BYTES] = {0};
    uint8 ucRespVal[FILE_LEN_BYTES] = {0};
    bool blStatus = false;
    bool blSendResult = false;
    bool blRecvResult = false;
    bool blValidResp = false;

    // Input parameter validation
    if (pulMaxChunk != NULL)
    {
        // Encode file length as byte array (little endian)
        memcpy(ucFileLen, &ulFileLen, FILE_LEN_BYTES);

        // Prepare and send announce frame
        stAnnounce.ucCmd      = CMD_INIT;
        stAnnounce.ucType     = TYPE_FILE_LENGTH;
        stAnnounce.ulLength   = FILE_LEN_BYTES;
        stAnnounce.ulSeqNum   = SEQ_INIT;
        stAnnounce.pucValue   = ucFileLen;
        stAnnounce.ucChecksum = DataCalcChecksum(ucFileLen, stAnnounce.ulLength);

        blSendResult = DataSendFrame(&stAnnounce);

        if (blSendResult)
        {
            // Wait for response from receiver (max chunk size)
            blRecvResult = DataReceiveFrame(&stResp, ucRespVal, FILE_LEN_BYTES,
            								DATA_SENDER_TIMEOUT_MS);

            if (blRecvResult)
            {
                blValidResp = ((stResp.ucCmd    == CMD_INIT) &&
                               (stResp.ucType   == TYPE_CHUNK_SIZE) &&
                               (stResp.ulLength == FILE_LEN_BYTES));

                if (blValidResp)
                {
                    // Copy chunk size value from response
                    memcpy(pulMaxChunk, ucRespVal, FILE_LEN_BYTES);
                    blStatus = true;
                }
            }
        }
    }

    return blStatus;
}

//********************* .FileTransfer ******************************************
// Purpose : Send file data in chunks with TLV protocol and sequence number
// Inputs  : pucData    - Pointer to data buffer
//           ulLen      - Length of data
//           ulMaxChunk - Max chunk size allowed by receiver
// Outputs : TRUE if all chunks sent, FALSE if error
// Notes   : No early returns; all variables declared at top
// *****************************************************************************
static bool FileTransfer(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk)
{
    bool   blStatus     = false;
    bool   blSendResult = false;
    uint32 ulSeqNum     = 1; // Start sequence number from 1
    uint32 ulOffset     = 0;
    uint8  ucChunkBuf[MAX_FRAME_SIZE] = {0};
    uint32 ulChunk      = 0U;

    // Input parameter validation
    if ((pucData != NULL) && (ulLen > 0U) && (ulMaxChunk > 0U))
    {
        while (ulOffset < ulLen)
        {
            ulChunk = (ulLen - ulOffset > ulMaxChunk) ? ulMaxChunk : 
                      (ulLen - ulOffset);

            if (ulChunk > sizeof(ucChunkBuf))
            {
                printf("Error: ulChunk size exceeds ucChunkBuf\r\n");
                blStatus = false;
                break;
            }

            memcpy(ucChunkBuf, &pucData[ulOffset], ulChunk);

            DATA_FRAME stData = {0};
            stData.ucCmd      = CMD_TRANSFER;
            stData.ucType     = TYPE_DATA;
            stData.ulLength   = ulChunk;
            stData.ulSeqNum   = ulSeqNum;
            stData.pucValue   = ucChunkBuf;
            stData.ucChecksum = DataCalcChecksum(ucChunkBuf, ulChunk);

            blSendResult = DataSendFrame(&stData);

            if (!blSendResult)
            {
                printf("Error: DataSendFrame failed at chunk %lu\r\n", ulSeqNum);
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
    else
    {
        blStatus = false;
    }

    return blStatus;
}

//EOF
