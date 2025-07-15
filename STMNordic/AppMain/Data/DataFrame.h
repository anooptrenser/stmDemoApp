//******************************* DataFrame ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
// *****************************************************************************
//
// File     : DataFrame.h
// Summary  : TLV frame structure, enums, and API for build/parse
// Note     : None
// Author   : Anoop G
// Date     : 14-07-2025
//
// *****************************************************************************

#ifndef DATAFRAME_H
#define DATAFRAME_H

//***************************** Include Files **********************************
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

//***************************** Global Constants *******************************
#define MAX_FRAME_SIZE 1024
#define FRAME_HEADER_SIZE 10U

//***************************** Enums ******************************************
typedef enum
{
    CMD_INIT = 0x01,
    CMD_TRANSFER = 0x02
} COMMAND_TYPE;

typedef enum
{
    TYPE_FILE_LENGTH = 0x01,
    TYPE_CHUNK_SIZE  = 0x02,
    TYPE_DATA        = 0x03,
    TYPE_ACK         = 0x00
} TYPE_TYPE;

//***************************** Data Structures ********************************
// TLV Frame with Sequence Number
typedef struct
{
    uint8  ucCmd;
    uint8  ucType;
    uint32 ulLength;
    uint32 ulSeqNum;   // Sequence number for tracking packets
    uint8* pucValue;
    uint8  ucChecksum;
} DATA_FRAME;

//***************************** Function Declarations **************************
uint8 DataCalcChecksum(const uint8* pucData, uint32 ulLen);
uint32 BuildDataFrame(const DATA_FRAME* psFrame, uint8* buffer, uint32 maxBufSize);
bool DataSendFrame(const DATA_FRAME* psFrame);
bool DataReceiveFrame(DATA_FRAME* psFrame, uint8* pucValueBuffer, uint32 ulMaxLen, uint32 ulTimeoutMs);
void ParseHeader(const uint8* ucHeader, DATA_FRAME* psFrame);

#endif // DATAFRAME_H

// EOF