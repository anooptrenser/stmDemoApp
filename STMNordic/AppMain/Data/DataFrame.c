//******************************* DataFrame ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
// *****************************************************************************
//
// File     : DataFrame.c
// Summary  : TLV frame build/parse and checksum implementation
// Note     : None
// Author   : Anoop G
// Date     : 14-07-2025
//
// *****************************************************************************

//********************* Include Files *******************************************
#include <stddef.h>
#include <string.h>
#include "DataFrame.h"
#include "UartDriver.h"
#include "common.h"

//********************* DataCalcChecksum ***************************************
// Purpose : Calculate checksum (sum of all bytes in buffer)
// Inputs  : pucData - Pointer to data buffer
//           ulLen   - Number of bytes to consider
// Outputs : None
// Returns : uint8   - Checksum (sum of bytes modulo 256)
// Notes   : Returns 0 if pucData is NULL
// *****************************************************************************
uint8 DataCalcChecksum(const uint8* pucData, uint32 ulLen)
{
    uint8 ucSum = 0;
    uint32 ulIdx = 0;

    if (pucData == NULL)
    {
        return 0;
    }

    for (ulIdx = 0; ulIdx < ulLen; ++ulIdx)
    {
        ucSum += pucData[ulIdx];
    }

    return ucSum;
}
//********************* DataSendFrame ******************************************
// Purpose : Serialize and send a TLV frame with sequence number
// Inputs  : psFrame - Pointer to fully-populated DATA_FRAME
// Outputs : None
// Returns : TRUE if sent successfully, FALSE otherwise
// Notes   : Calls BuildDataFrame for frame packing
// *****************************************************************************
bool DataSendFrame(const DATA_FRAME* psFrame)
{
    bool   blStatus  = false;
    uint8  buffer[MAX_FRAME_SIZE];
    uint32 frameLen  = 0U;

    if (psFrame != NULL)
    {
        frameLen = BuildDataFrame(psFrame, buffer, sizeof(buffer));

        if (frameLen != 0U)
        {
            if (UartSend(buffer, frameLen))
            {
                blStatus = true;
            }
        }
    }

    return blStatus;
}

//********************* BuildDataFrame *****************************************
// Purpose : Build a TLV frame buffer from struct fields
// Inputs  : psFrame    - Pointer to DATA_FRAME to serialize
//           buffer     - Buffer to fill with serialized frame
//           maxBufSize - Available size in buffer (bytes)
// Outputs : None
// Returns : Number of bytes written to buffer (total frame length)
// Notes   : Uses memcpy for multi-byte fields; returns 0 on error
// *****************************************************************************
uint32 BuildDataFrame(const DATA_FRAME* psFrame, uint8* buffer, uint32 maxBufSize)
{
    uint32 totalLen = 0U;

    if ((psFrame != NULL) && (buffer != NULL))
    {
    	totalLen = FRAME_HEADER_SIZE + psFrame->ulLength + 1U; // 1U for checksum

        if (totalLen <= maxBufSize)
        {
            // Header: cmd, type, length (4), seqNum (4)
            buffer[0] = psFrame->ucCmd;
            buffer[1] = psFrame->ucType;
            memcpy(&buffer[2], &psFrame->ulLength, 4U);  // Little-endian length
            memcpy(&buffer[6], &psFrame->ulSeqNum, 4U);  // Little-endian seqNum

            // Copy Value
            if ((psFrame->ulLength > 0U) && (psFrame->pucValue != NULL))
            {
                memcpy(&buffer[FRAME_HEADER_SIZE], psFrame->pucValue, psFrame->ulLength);
            }

            // Append checksum
            buffer[FRAME_HEADER_SIZE + psFrame->ulLength] = psFrame->ucChecksum;
        }
        else
        {
            totalLen = 0U; // Frame too large
        }
    }

    return totalLen;
}

//********************* DataReceiveFrame ***************************************
// Purpose : Receive and parse a TLV frame with sequence number
// Inputs  : psFrame     - Pointer to DATA_FRAME to fill with received fields
//           pucBuffer   - Buffer to store received value payload
//           ulMaxLen    - Maximum value size that pucBuffer can hold
//           ulTimeoutMs - Timeout for receive operation (ms)
// Outputs : None
// Returns : TRUE if frame received and valid, FALSE otherwise
// Notes   : Parses and validates frame; uses helper for header fields
// *****************************************************************************
bool DataReceiveFrame(DATA_FRAME* psFrame, uint8* pucBuffer, uint32 ulMaxLen, uint32 ulTimeoutMs)
{
    bool   blStatus     = false;
    bool   blHeaderOk   = false;
    bool   blDataOk     = false;
    uint8  ucHeader[FRAME_HEADER_SIZE];
    uint32 totalToReceive = 0U;
    uint8  ucCalcCsum = 0U;

    if ((psFrame != NULL) && (pucBuffer != NULL))
    {
        // Step 1: Receive header
        blHeaderOk = UartReceive(ucHeader, sizeof(ucHeader), ulTimeoutMs);

        if (blHeaderOk)
        {
            ParseHeader(ucHeader, psFrame);
            // Step 2: Check length validity
            if (psFrame->ulLength <= ulMaxLen)
            {
                totalToReceive = psFrame->ulLength + 1U;

                // Step 3: Receive value + checksum
                blDataOk = UartReceive(pucBuffer, totalToReceive, ulTimeoutMs);

                if (blDataOk)
                {
                    psFrame->pucValue   = (psFrame->ulLength > 0U) ? pucBuffer : NULL;
                    psFrame->ucChecksum = pucBuffer[psFrame->ulLength];

                    if ((psFrame->ulLength > 0U) && (psFrame->pucValue != NULL))
                    {
                        ucCalcCsum = DataCalcChecksum(psFrame->pucValue, psFrame->ulLength);
                    }

                    if (psFrame->ucChecksum == ucCalcCsum)
                    {
                        blStatus = true; // All OK
                    }
                }
            }
        }
    }

    return blStatus;
}

//********************* ParseHeader ********************************************
// Purpose : Extract header fields from 10-byte buffer into DATA_FRAME
// Inputs  : ucHeader - 10-byte header buffer
//           psFrame  - Pointer to DATA_FRAME to fill
// Outputs : None
// Returns : None
// Notes   : Uses memcpy for little-endian 32-bit fields
// *****************************************************************************
void ParseHeader(const uint8* ucHeader, DATA_FRAME* psFrame)
{
    if ((ucHeader != NULL) && (psFrame != NULL))
    {
        psFrame->ucCmd  = ucHeader[0];
        psFrame->ucType = ucHeader[1];
        memcpy(&psFrame->ulLength,  &ucHeader[2], 4U);  // Length: byte 2-5
        memcpy(&psFrame->ulSeqNum,  &ucHeader[6], 4U);  // SeqNum: byte 6-9
    }
}


//EOF
