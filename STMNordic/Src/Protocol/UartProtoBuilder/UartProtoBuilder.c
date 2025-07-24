//******************************* UartProtoBuilder ****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartProtoBuilder.c
// Summary  : Serial frame building and serialization implementation
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include <string.h>
#include <stdbool.h>
#include "UartProtoBuilder.h"
#include "UartDriver.h"
#include "AppMain.h"
#include "Tmp.h"

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Calculate 8-bit checksum of a provided buffer
// Inputs   : pucData   - Source data buffer pointer
//            ulLen     - Number of bytes to sum
// Outputs  : None
// Returns  : uint8     - Calculated checksum
//*****************************************************************************
uint8 UartProtoCalcChecksum(const uint8* pucData, uint32 ulLen)
{
    uint8 ucSum = 0U;
    uint32 ulIdx = 0U;

    if (pucData != NULL)
    {
        for (ulIdx = 0U; ulIdx < ulLen; ++ulIdx)
        {
            ucSum += pucData[ulIdx];
        }
    }

    return ucSum;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Serializes a data frame into a buffer
// Inputs   : psFrame     - Frame to serialize
//            buffer      - Output buffer
//            maxBufSize  - Max allowed output size
// Outputs  : None
// Returns  : uint32      - Number of bytes written, 0 if error
//*****************************************************************************
uint32 UartProtoBuildFrame(const DATA_FRAME* psFrame, uint8* ucBuffer, uint32 ulMaxBufSize)
{
    uint32 ulTotalLen = 0U;

    if ((psFrame != NULL) && (ucBuffer != NULL))
    {
    	ulTotalLen = FRAME_HEADER_SIZE + psFrame->ulLength + 1U;

        if (ulTotalLen <= ulMaxBufSize)
        {
        	ucBuffer[0] = psFrame->ucCmd;
        	ucBuffer[1] = psFrame->ucType;
            memcpy(&ucBuffer[2], &psFrame->ulLength, 4U);
            memcpy(&ucBuffer[6], &psFrame->unSeqNum, 2U);

            if ((psFrame->ulLength > 0U) && (psFrame->pucValue != NULL))
            {
                memcpy(&ucBuffer[FRAME_HEADER_SIZE], psFrame->pucValue, psFrame->ulLength);
            }

            ucBuffer[FRAME_HEADER_SIZE + psFrame->ulLength] = psFrame->ucChecksum;
        }
        else
        {
        	ulTotalLen = 0U;
        }
    }

    return ulTotalLen;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Sends a framed data packet over UART with start/stop markers
// Inputs  : psFrame - Pointer to frame to send
// Outputs : None
// Returns : bool    - TRUE if frame sent successfully, FALSE otherwise
//*****************************************************************************
bool UartProtoSendFrame(const DATA_FRAME* psFrame)
{
    bool blStatus = false;
    uint8 ucFrameBuf[MAX_FRAME_SIZE + FRAME_HEADER_SIZE + 1] = {0}; // Frame only
    uint8 ucRawBuf[MAX_RAW_FRAME_LEN] = {0};                   
    uint32 ulFrameLen = 0U;
    uint32 ulTotalLen = 0U;

    if (psFrame != NULL)
    {
        // Build the unframed raw payload
        ulFrameLen = UartProtoBuildFrame(psFrame, ucFrameBuf, sizeof(ucFrameBuf));

        if (ulFrameLen != 0U)
        {
            // Wrap the frame with start/stop bytes
            ulTotalLen = ulFrameLen + 2U;
            ucRawBuf[0] = UART_START_BYTE;
            memcpy(&ucRawBuf[1], ucFrameBuf, ulFrameLen);
            ucRawBuf[ulFrameLen + 1] = UART_STOP_BYTE;

            // Print for debugging
            printf("CMD=0x%02X, TYPE=0x%02X, LEN=%lu, SEQ=%u, CHK=0x%02X",
                       psFrame->ucCmd,
                       psFrame->ucType,
                       psFrame->ulLength,
                       psFrame->unSeqNum,
                       psFrame->ucChecksum);
            HexDump(ucRawBuf, ulTotalLen);

            // Send over UART
            blStatus = UartSend(ucRawBuf, ulTotalLen);

            if (!blStatus)
            {
                printf("UartSend Failed!");
            }
            else
            {
                printf("Frame sent successfully (%lu bytes)", ulTotalLen);
            }
        }
        else
        {
            printf("UartProtoBuildFrame returned zero length.");
        }
    }
    else
    {
        printf("SendDataFrame: NULL psFrame!");
    }

    return blStatus;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Sends a data chunk as a frame with sequence number and checksum
// Inputs   : unSeqNum  - Sequence number for the chunk
//          : pucData   - Pointer to chunk data
//          : ulLength  - Length of chunk data
// Outputs  : None
// Returns  : bool      - TRUE if frame sent successfully, FALSE otherwise
//*****************************************************************************
bool UartProtoSendChunk(const uint16 unSeqNum, const uint8* pucData, const uint32 ulLength)
{
    DATA_FRAME stData = {0};
    uint8 ucChecksum = UartProtoCalcChecksum(pucData, ulLength);

    stData.ucCmd = CMD_TRANSFER;
    stData.ucType = TYPE_DATA;
    stData.ulLength = ulLength;
    stData.unSeqNum = unSeqNum;
    stData.pucValue = (uint8*)pucData;
    stData.ucChecksum = ucChecksum;

    if (!UartProtoSendFrame(&stData))
    {
        printf("Error: SendDataFrame failed at seq %u\r\n", unSeqNum);
        return false;
    }

    return true;
}

// EOF
