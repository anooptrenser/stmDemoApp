//*******************************DataFrame*************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : DataFrame.c
//Summary  : TLV frame build/parse and checksum implementation
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stddef.h>
#include <string.h>
#include "DataFrame.h"
#include "UartDriver.h"
#include "common.h"

//*********************.DataCalcChecksum.**************************************
//Purpose : Calculate checksum (sum of all bytes)
//Inputs  : pucData - pointer to data
//          ulLen   - number of bytes
//Outputs : None
//Return  : uint8 checksum
//Notes   : None
//*****************************************************************************
uint8 DataCalcChecksum(const uint8* pucData, uint32 ulLen)
{
    uint8 ucSum = 0;
    uint32 ulIdx = 0;

    for (ulIdx = 0; ulIdx < ulLen; ++ulIdx)
    {
        ucSum += pucData[ulIdx];
    }

    return ucSum;
}

//*********************.DataSendFrame.*****************************************
//Purpose : Serialize and send a TLV frame with sequence number
//Inputs  : psFrame - pointer to DATA_FRAME
//Outputs : None
//Return  : TRUE if sent successfully, FALSE otherwise
//Notes   : None
//*****************************************************************************
bool DataSendFrame(const DATA_FRAME* psFrame)
{
    uint32 totalLen = 10 + psFrame->ulLength + 1;
    uint8 buffer[MAX_FRAME_SIZE];

    // Build header
    buffer[0] = psFrame->ucCmd;
    buffer[1] = psFrame->ucType;
    buffer[2] = (uint8)((psFrame->ulLength >> 0) & 0xFF);
    buffer[3] = (uint8)((psFrame->ulLength >> 8) & 0xFF);
    buffer[4] = (uint8)((psFrame->ulLength >> 16) & 0xFF);
    buffer[5] = (uint8)((psFrame->ulLength >> 24) & 0xFF);
    buffer[6] = (uint8)((psFrame->ulSeqNum >> 0) & 0xFF);
    buffer[7] = (uint8)((psFrame->ulSeqNum >> 8) & 0xFF);
    buffer[8] = (uint8)((psFrame->ulSeqNum >> 16) & 0xFF);
    buffer[9] = (uint8)((psFrame->ulSeqNum >> 24) & 0xFF);

    // Copy value
    if ((psFrame->ulLength > 0) && (psFrame->pucValue != NULL))
    {
        memcpy(&buffer[10], psFrame->pucValue, psFrame->ulLength);
    }

    // Add checksum
    buffer[10 + psFrame->ulLength] = psFrame->ucChecksum;

    // Send the entire frame in one call
    if (!UartSend(buffer, totalLen))
    {
        return false;
    }

    return true;
}

//*********************.DataReceiveFrame.**************************************
//Purpose : Receive and parse a TLV frame with sequence number
//Inputs  : psFrame        - pointer to DATA_FRAME to fill
//          pucBuffer      - buffer for value field
//          ulMaxLen       - max allowed value length
//          ulTimeoutMs    - receive timeout in ms
//Outputs : None
//Return  : TRUE if frame received and valid, FALSE otherwise
//Notes   : Calls helper functions for each step
//*****************************************************************************
bool DataReceiveFrame(DATA_FRAME* psFrame, uint8* pucBuffer, uint32 ulMaxLen, uint32 ulTimeoutMs)
{
    uint8 ucHeader[10];

    // Receive header (cmd, type, length, seqNum)
    if (!UartReceive(ucHeader, 10, ulTimeoutMs))
    {
        return false;
    }
    // Parse header
    psFrame->ucCmd    = ucHeader[0];
    psFrame->ucType   = ucHeader[1];
    psFrame->ulLength = (uint32)ucHeader[2] | ((uint32)ucHeader[3]<<8) | ((uint32)ucHeader[4]<<16) | ((uint32)ucHeader[5]<<24);
    psFrame->ulSeqNum = (uint32)ucHeader[6] | ((uint32)ucHeader[7]<<8) | ((uint32)ucHeader[8]<<16) | ((uint32)ucHeader[9]<<24);

    if (psFrame->ulLength > ulMaxLen)
    {
        return false;
    }

    // Receive value and checksum in a single call
    uint32 totalToReceive = psFrame->ulLength + 1; // value + checksum
    if (!UartReceive(pucBuffer, totalToReceive, ulTimeoutMs))
    {
        return false;
    }

    // Assign value and checksum
    psFrame->pucValue = (psFrame->ulLength > 0) ? pucBuffer : NULL;
    psFrame->ucChecksum = pucBuffer[psFrame->ulLength];

    // Validate checksum
    uint8 ucCalcCsum = 0;
    if ((psFrame->ulLength > 0) && (psFrame->pucValue != NULL))
    {
        ucCalcCsum = DataCalcChecksum(psFrame->pucValue, psFrame->ulLength);
    }

    if (psFrame->ucChecksum != ucCalcCsum)
    {
    	return false;
    }

    return true;
}

//EOF

