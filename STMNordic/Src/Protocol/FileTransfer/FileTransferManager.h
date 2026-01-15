//******************************* FileTransferManager *************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FileTransferManager.h
// Summary  : High-level file transfer task header
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************

#ifndef FILE_TRANSFER_MANAGER_H
#define FILE_TRANSFER_MANAGER_H

//*************************** Include Files ***********************************
#include <stdbool.h>
#include "common.h"
//******************************* Global Types ******************************** 

//***************************** Global Variables ****************************** 

//*************************** Global Constants ********************************
#define DATA_SENDER_TIMEOUT_MS  5000
#define FILE_LEN_BYTES          4
#define SEQ_INIT                0
#define MAX_ACK_RETRIES         3

//************************** Function Declarations ****************************
bool FileTransferManager(const uint8* pucData, uint32 ulLen);

#endif // FILE_TRANSFER_MANAGER_H

// EOF
