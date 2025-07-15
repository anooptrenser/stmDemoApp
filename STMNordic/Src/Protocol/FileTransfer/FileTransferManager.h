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

#include "common.h"
#include <stdbool.h>

//*************************** Global Constants ********************************
#define DATA_SENDER_TIMEOUT_MS  (2000U)
#define FILE_LEN_BYTES          (4U)
#define SEQ_INIT                (0U)

//************************** Function Declarations ****************************
bool FileTransferManager(void);

#endif // FILE_TRANSFER_MANAGER_H

// EOF
