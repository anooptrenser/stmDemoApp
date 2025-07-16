//******************************* FrameParser *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FrameParser.c
// Summary  : Serial frame parsing and extraction implementation
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include <string.h>
#include <stdbool.h>
#include "FrameParser.h"
#include "UartDriver.h"
#include "common.h"

//************************** Forward Declarations *****************************
static bool ReceiveHeader(uint8* ucHeader, uint32 ulTimeoutMs);
static bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 totalToReceive,
									 uint32 ulTimeoutMs);
static bool ValidateAndExtractValue(DATA_FRAME* psFrame, const uint8* pucBuffer);
//*****************************************************************************
// Function : ParseHeader
// Purpose  : Populates a DATA_FRAME struct from header buffer
// Inputs   : ucHeader  - Received buffer
//            psFrame   - Frame to fill
// Outputs  : None
// Returns  : None
//*****************************************************************************
void ParseHeader(const uint8* ucHeader, DATA_FRAME* psFrame)
{
    if ((ucHeader != NULL) && (psFrame != NULL))
    {
        psFrame->ucCmd = ucHeader[0];
        psFrame->ucType = ucHeader[1];
        memcpy(&psFrame->ulLength, &ucHeader[2], 4U);
        memcpy(&psFrame->ulSeqNum, &ucHeader[6], 4U);
    }
}

//*****************************************************************************
// Function : ReceiveDataFrame
// Purpose  : Receives and parses a TLV frame from UART
// Inputs   : psFrame       - Frame structure to fill
//            pucBuffer     - Content/value storage buffer
//            ulMaxLen      - Max valid value length
//            ulTimeoutMs   - UART receive timeout (ms)
// Outputs  : None
// Returns  : bool          - TRUE for valid/correct frame, FALSE otherwise
//*****************************************************************************
bool ReceiveDataFrame(DATA_FRAME* psFrame, uint8* pucBuffer, uint32 ulMaxLen,
					  uint32 ulTimeoutMs)
{
    bool blStatus = false;
    uint8 ucHeader[FRAME_HEADER_SIZE];
    uint32 ulReceive = 0U;

    if ((psFrame != NULL) && (pucBuffer != NULL))
    {
        if (ReceiveHeader(ucHeader, ulTimeoutMs))
        {
            ParseHeader(ucHeader, psFrame);

            if (psFrame->ulLength <= ulMaxLen)
            {
            	ulReceive = psFrame->ulLength + 1U;

                if (ReceiveValueWithChecksum(pucBuffer, ulReceive, ulTimeoutMs))
                {
                    if (ValidateAndExtractValue(psFrame, pucBuffer))
                    {
                        blStatus = true;
                    }
                }
            }
        }
    }

    return blStatus;
}

//*****************************************************************************
// Function : ReceiveHeader
// Purpose  : Receives a frame header from UART
// Inputs   : ucHeader    - Header buffer to fill
//            ulTimeoutMs - Timeout in milliseconds
// Outputs  : None
// Returns  : bool        - TRUE if successful, FALSE otherwise
//*****************************************************************************
static bool ReceiveHeader(uint8* ucHeader, uint32 ulTimeoutMs)
{
    return UartReceive(ucHeader, FRAME_HEADER_SIZE, ulTimeoutMs);
}

//*****************************************************************************
// Function : ReceiveValueWithChecksum
// Purpose  : Receives value + checksum bytes from UART
// Inputs   : pucBuffer   - Buffer to fill with value + checksum
//            totalToReceive - Bytes to receive
//            ulTimeoutMs - Timeout in ms
// Outputs  : None
// Returns  : bool        - TRUE if success, FALSE otherwise
//*****************************************************************************
static bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 totalToReceive,
									 uint32 ulTimeoutMs)
{
    return UartReceive(pucBuffer, totalToReceive, ulTimeoutMs);
}

//*****************************************************************************
// Function : ValidateAndExtractValue
// Purpose  : Fills value pointer, extracts checksum, and validates integrity
// Inputs   : psFrame     - Frame to fill
//            pucBuffer   - Buffer containing value and checksum
// Outputs  : None
// Returns  : bool        - TRUE if checksum matches, FALSE otherwise
//*****************************************************************************
static bool ValidateAndExtractValue(DATA_FRAME* psFrame, const uint8* pucBuffer)
{
    bool blValid = false;
    uint8 ucCalcCsum = 0U;

    if (psFrame->ulLength > 0U)
    {
        psFrame->pucValue = (uint8*)pucBuffer;
    }
    else
    {
        psFrame->pucValue = NULL;
    }

    psFrame->ucChecksum = pucBuffer[psFrame->ulLength];

    if ((psFrame->ulLength > 0U) && (psFrame->pucValue != NULL))
    {
        ucCalcCsum = CalcChecksum(psFrame->pucValue, psFrame->ulLength);
    }
    else
    {
        ucCalcCsum = 0U;
    }

    if (psFrame->ucChecksum == ucCalcCsum)
    {
        blValid = true;
    }

    return blValid;
}

// EOF
