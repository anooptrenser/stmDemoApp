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

//*****************************************************************************
// Function : ValidateChecksum
// Purpose  : Validates the checksum of a buffer using CalcChecksum
// Inputs   : pucValue           - Data buffer pointer
//            ulLength           - Number of bytes in buffer
//            ucReceivedChecksum - Received checksum to validate against
// Outputs  : None
// Returns  : bool               - TRUE if checksum matches, FALSE otherwise
//*****************************************************************************
bool ValidateChecksum(const uint8* pucValue, uint32 ulLength, uint8 ucReceivedChecksum)
{
    uint8 calc = CalcChecksum(pucValue, ulLength);
    return (calc == ucReceivedChecksum);
}

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
// Purpose  : Receives the fixed-length 8-byte UART header.
// Inputs   : pucHeader   - Pointer to the buffer where header will be stored.
//            ulTimeoutMs - Timeout for UART receive in milliseconds.
// Outputs  : None
// Returns  : bool        - TRUE if header received successfully.
//                        - FALSE if timeout or invalid pointer.
//*****************************************************************************
bool ReceiveHeader(uint8* pucHeader, uint32 ulTimeoutMs)
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
bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 ulToReceive, uint32 ulTimeoutMs)
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
bool ValidateAndExtractValue(DATA_FRAME* psFrame, const uint8* pucBuffer)
{
    bool blValid = false;

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

        // Use ValidateChecksum for validation
        blValid = ValidateChecksum(psFrame->pucValue, psFrame->ulLength, psFrame->ucChecksum);
    }

    return blValid;
}

// EOF
