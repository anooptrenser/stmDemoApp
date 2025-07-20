//******************************* FileTransferHelper *************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferHelper.c
// Summary  : Helper functions for FileTransferManager (ACK, chunk, frame)
// Author   : Anoop G
// Date     : 20-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include "FileTransferHelper.h"
#include "FileTransferManager.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "UartDriver.h"
#include <stdio.h>
#include <string.h>

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for ACK for a given sequence number from receiver
// Inputs   : unExpectedSeqNum - Expected ACK sequence number
//          : ulTimeoutMs      - Timeout for ACK wait
// Outputs  : None
// Returns  : bool             - TRUE if correct ACK received, FALSE otherwise
//*****************************************************************************
bool WaitForAck(uint16 unExpectedSeqNum, uint32 ulTimeoutMs)
{
    DATA_FRAME stAck = {0};
    uint8 ackBuf[1] = {0}; // Length 0 payload for ACK
    bool blStatus = false;

    blStatus = ReceiveDataFrame(&stAck, ackBuf, sizeof(ackBuf), ulTimeoutMs);

    if (blStatus == true)
    {
        if ((stAck.ucCmd == CMD_TRANSFER) &&
            (stAck.ucType == TYPE_ACK) &&
            (stAck.unSeqNum == unExpectedSeqNum) &&
            (stAck.ulLength == 0U))
        {
            blStatus = true;
        }
        else
        {
            blStatus = false;
        }
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
bool SendChunk(const uint16 unSeqNum, const uint8* pucData, const uint32 ulLength)
{
    DATA_FRAME stData = {0};
    uint8 ucChecksum = CalcChecksum(pucData, ulLength);

    stData.ucCmd = CMD_TRANSFER;
    stData.ucType = TYPE_DATA;
    stData.ulLength = ulLength;
    stData.unSeqNum = unSeqNum;
    stData.pucValue = (uint8*)pucData;
    stData.ucChecksum = ucChecksum;

    if (!SendDataFrame(&stData))
    {
        printf("Error: SendDataFrame failed at seq %u\r\n", unSeqNum);
        return false;
    }

    return true;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for ACK with retry mechanism for a given sequence number
// Inputs   : unSeqNum - Sequence number to wait for ACK
// Outputs  : None
// Returns  : bool     - TRUE if ACK received within retries, FALSE otherwise
//*****************************************************************************
bool WaitForAckWithRetry(const uint16 unSeqNum)
{
    bool blAckReceived = false;

    for (int nRetry = 0; nRetry < MAX_ACK_RETRIES; ++nRetry)
    {
        if (WaitForAck(unSeqNum, DATA_SENDER_TIMEOUT_MS))
        {
            blAckReceived = true;
            break;
        } else {
            printf("Warning: ACK not received for seq %u (retry %d/%d)\r\n",
                   unSeqNum, nRetry + 1, MAX_ACK_RETRIES);
        }
    }

    if (!blAckReceived)
    {
        printf("Error: Retries exceeded for seq %u\r\n", unSeqNum);
    }

    return blAckReceived;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Receives a complete UART frame (header + payload + checksum)
//            and fills a DATA_FRAME struct with parsed values.
// Inputs   : psFrame      - Pointer to DATA_FRAME structure to fill.
//          : pucBuffer    - Buffer to hold payload + checksum data.
//          : ulMaxLen     - Maximum allowed length of payload.
//          : ulTimeoutMs  - Timeout for UART reception (in milliseconds).
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
