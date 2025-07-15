//*******************************DataSender************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : DataSender.h
//Summary  : High-level TLV sender protocol logic header
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

#ifndef DATASENDER_H
#define DATASENDER_H

//*********************Include Files*******************************************
#include <stdbool.h>

//********************* Global Constants *****************************************
#define DATA_SENDER_TIMEOUT_MS    (2000U)
#define FILE_LEN_BYTES            (4U)
#define SEQ_INIT                  (0U)

//*********************Forward Declarations************************************
bool DataSender(void);

#endif // DATASENDER_H

//EOF