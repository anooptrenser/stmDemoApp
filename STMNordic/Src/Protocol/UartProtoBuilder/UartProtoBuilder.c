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

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Calculate 8-bit checksum of a provided buffer
// Inputs   : pucData   - Source data buffer pointer
//            ulLen     - Number of bytes to sum
// Outputs  : None
// Returns  : uint8     - Calculated checksum
//*****************************************************************************
uint8 CalcChecksum(const uint8* pucData, uint32 ulLen)
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
uint32 BuildDataFrame(const DATA_FRAME* psFrame, uint8* ucBuffer, uint32 ulMaxBufSize)
{
    uint32 ultotalLen = 0U;

    if ((psFrame != NULL) && (ucBuffer != NULL))
    {
    	ultotalLen = FRAME_HEADER_SIZE + psFrame->ulLength + 1U;

        if (ultotalLen <= ulMaxBufSize)
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
        	ultotalLen = 0U;
        }
    }

    return ultotalLen;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Sends a data frame over UART
// Inputs   : psFrame     - Frame to send
// Outputs  : None
// Returns  : bool        - TRUE for success, FALSE for error
//*****************************************************************************
bool SendDataFrame(const DATA_FRAME* psFrame)
{
    bool blStatus = false;
    uint8 ucBuffer[MAX_FRAME_SIZE] = {0};
    uint32 ulFrameLen = 0U;

    if (psFrame != NULL)
    {
    	ulFrameLen = BuildDataFrame(psFrame, ucBuffer, sizeof(ucBuffer));

        if (ulFrameLen != 0U)
        {
            if (UartSend(ucBuffer, ulFrameLen))
            {
                blStatus = true;
            }
        }
    }

    return blStatus;
}

// EOF
