//******************************* Parser **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Parser.c
// Summary  : Serial frame parsing and extraction implementation
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include <string.h>
#include <stdbool.h>
#include "Parser.h"
#include "UartDriver.h"
#include "common.h"

//************************** Forward Declarations *****************************
static bool ReceiveHeader(uint8* pucHeader, uint32 ulTimeoutMs);
static bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 ulToReceive,
									 uint32 ulTimeoutMs);
static bool ValidateAndExtractValue(DATA_FRAME* psFrame, const uint8* pucBuffer);

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Decodes the raw 8-byte UART header into a structured DATA_FRAME.
// Inputs   : pucHeader  - Pointer to the 8-byte received header buffer.
//            psFrame    - Pointer to the DATA_FRAME struct to populate.
// Outputs  : None
// Returns  : None
//*****************************************************************************
void ParseHeader(const uint8* pucHeader, DATA_FRAME* psFrame)
{
    if ((pucHeader != NULL) && (psFrame != NULL))
    {
        psFrame->ucCmd = pucHeader[0];
        psFrame->ucType = pucHeader[1];
        memcpy(&psFrame->ulLength, &pucHeader[2], 4U);
        memcpy(&psFrame->unSeqNum, &pucHeader[6], 2U);
    }
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Receives a complete UART frame (header + payload + checksum)
//            and fills a DATA_FRAME struct with parsed values.
// Inputs   : psFrame      - Pointer to DATA_FRAME structure to fill.
//            pucBuffer    - Buffer to hold payload + checksum data.
//            ulMaxLen     - Maximum allowed length of payload.
//            ulTimeoutMs  - Timeout for UART reception (in milliseconds).
// Outputs  : None
// Returns  : bool         - TRUE if a valid frame is received and verified.
//                         - FALSE if timeout/error/invalid checksum occurs.
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

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Receives the fixed-length 8-byte UART header.
// Inputs   : pucHeader   - Pointer to the buffer where header will be stored.
//            ulTimeoutMs - Timeout for UART receive in milliseconds.
// Outputs  : None
// Returns  : bool        - TRUE if header received successfully.
//                        - FALSE if timeout or invalid pointer.
//*****************************************************************************
static bool ReceiveHeader(uint8* pucHeader, uint32 ulTimeoutMs)
{
    bool blResult = false;

    if (pucHeader != NULL)
    {
        blResult = UartReceive(pucHeader, FRAME_HEADER_SIZE, ulTimeoutMs);
    }

    return blResult;
}


//******************************.FUNCTION_HEADER.******************************
// Purpose  : Receives value + checksum bytes from UART
// Inputs   : pucBuffer   - Buffer to fill with value + checksum
//            totalToReceive - Bytes to receive
//            ulTimeoutMs - Timeout in ms
// Outputs  : None
// Returns  : bool        - TRUE if success, FALSE otherwise
//*****************************************************************************
static bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 ulToReceive, uint32 ulTimeoutMs)
{
    bool blResult = false;

    if (pucBuffer != NULL)
    {
        blResult = UartReceive(pucBuffer, ulToReceive, ulTimeoutMs);
    }

    return blResult;
}

//******************************.FUNCTION_HEADER.******************************
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

    if ((psFrame != NULL) && (pucBuffer != NULL))
    {
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
    }

    return blValid;
}

// EOF
