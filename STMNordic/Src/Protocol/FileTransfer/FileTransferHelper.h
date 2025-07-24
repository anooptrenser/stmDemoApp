//******************************* FileTransferHelper *************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferHelper.h
// Summary  : Helper function declarations for FileTransferManager
// Author   : Anoop G
// Date     : 20-07-2025
//
//*****************************************************************************
#ifndef FILE_TRANSFER_HELPER_H
#define FILE_TRANSFER_HELPER_H

//*********************Include Files*******************************************
#include "FileTransferManager.h"
#include <stdbool.h>
#include <stdint.h>
#include "UartProtoBuilder.h"

//************************** Function Declarations ****************************
bool WaitForAck(uint16 unExpectedSeqNum, uint32 ulTimeoutMs);
bool WaitForAckWithRetry(const uint16 unSeqNum);
bool WaitForFrameFromQueue(DATA_FRAME* psFrame, uint32 ulTimeoutMs);

#endif // FILE_TRANSFER_HELPER_H
