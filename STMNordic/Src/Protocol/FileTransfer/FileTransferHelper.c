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
#include <stdlib.h>

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
    bool blStatus = false;

    blStatus = WaitForFrameFromQueue(&stAck, ulTimeoutMs);

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

        if (stAck.pucValue != NULL)
        {
            free(stAck.pucValue);
            stAck.pucValue = NULL;
        }
    }

return blStatus;

}

//******************************.FUNCTION_HEADER.******************************
// Purpose  : Waits for a frame from the protocol queue with timeout
// Inputs   : psFrame      - Pointer to DATA_FRAME structure to fill
//          : ulTimeoutMs  - Timeout in milliseconds to wait for frame
// Outputs  : None
// Return   : bool         - TRUE if frame received, FALSE if timeout or error
// Notes    : Uses OSQueueRecv to receive from gFrameQueueHandle
//*****************************************************************************
bool WaitForFrameFromQueue(DATA_FRAME* psFrame, uint32 ulTimeoutMs)
{
    return OSQueueRecv(gFrameQueueHandle, psFrame, ulTimeoutMs);
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

//EOF
