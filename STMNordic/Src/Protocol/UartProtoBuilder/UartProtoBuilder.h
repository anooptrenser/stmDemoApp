//******************************* UartProtoBuilder ****************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartProtoBuilder.h
// Summary  : Serial frame building and serialization header
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
#ifndef UART_PROTO_BUILDER_H
#define UART_PROTO_BUILDER_H

//*********************Include Files*******************************************
#include "common.h"
#include <stdbool.h>

//*************************** Global Constants ********************************
#define MAX_FRAME_SIZE      256U
#define FRAME_HEADER_SIZE   8U

typedef enum {
    CMD_INIT      = 0x01,
    CMD_TRANSFER  = 0x02
} COMMAND_TYPE;

typedef enum {
    TYPE_FILE_LENGTH = 0x01,
    TYPE_CHUNK_SIZE  = 0x02,
    TYPE_DATA        = 0x03,
    TYPE_ACK         = 0x04
} TYPE_TYPE;

//************************** Data Structures **********************************
typedef struct {
    uint8 ucCmd;
    uint8 ucType;
    uint32 ulLength;
    uint16 unSeqNum;         
    uint8 *pucValue;
    uint8 ucChecksum;
} DATA_FRAME;


//************************** Function Declarations ****************************
uint8 CalcChecksum(const uint8* pucData, uint32 ulLen);
uint32 BuildDataFrame(const DATA_FRAME* psFrame, uint8* ucbuffer, uint32 maxBufSize);
bool SendDataFrame(const DATA_FRAME* psFrame);

#endif

// EOF
