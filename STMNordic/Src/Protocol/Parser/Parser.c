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
#include <stdio.h>
#include <stdbool.h>
#include "Parser.h"
#include "UartDriver.h"
#include "common.h"
#include <stdlib.h>

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Validates the checksum of a buffer using UartProtoCalcChecksum
// Inputs   : pucValue           - Data buffer pointer
//            ulLength           - Number of bytes in buffer
//            ucReceivedChecksum - Received checksum to validate against
// Outputs  : None
// Returns  : bool               - TRUE if checksum matches, FALSE otherwise
//*****************************************************************************
bool ParseValidateChecksum(const uint8* pucValue, uint32 ulLength, uint8 ucReceivedChecksum)
{
    uint8 calc = UartProtoCalcChecksum(pucValue, ulLength);
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
// Purpose : Allocate memory and copy payload data into frame structure.
// Inputs  : pFrame     - Pointer to DATA_FRAME struct to store payload pointer.
//           pucPayload - Pointer to raw payload data buffer.
// Outputs : None
// Return  : true if payload is empty or allocation and copy succeed,
//           false if memory allocation fails.
// Notes   : Caller must free pFrame->pucValue when no longer needed.
//           Handles zero-length payload as a special case.
//*****************************************************************************
bool ParsePayload(DATA_FRAME* pFrame, const uint8* pucPayload)
{
    if (pFrame->ulLength == 0)
    {
        pFrame->pucValue = NULL;
        return true;
    }

    pFrame->pucValue = malloc(pFrame->ulLength);
    if (pFrame->pucValue == NULL)
    {
        printf("[ERROR] Memory allocation failed\n\r");
        return false;
    }

    memcpy(pFrame->pucValue, pucPayload, pFrame->ulLength);
    return true;
}

// EOF
