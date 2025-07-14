//*******************************DataFrame*************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : DataFrame.h
//Summary  : TLV frame structure, enums, and API for build/parse
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

#ifndef DATAFRAME_H
#define DATAFRAME_H

//*********************Include Files*******************************************
#include <stdbool.h>
#include "common.h"

//***************************** Global Constants ******************************
#define MAX_FRAME_SIZE 1024

typedef enum
{
    CMD_ANNOUNCE = 0x01,
    CMD_TRANSFER = 0x02
} COMMAND_TYPE;

typedef enum
{
    TYPE_FILE_LENGTH = 0x01,
    TYPE_CHUNK_SIZE  = 0x02,
    TYPE_DATA        = 0x03,
    TYPE_ACK         = 0x00
} TYPE_TYPE;

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

//*********************Forward Declarations************************************
uint8 DataCalcChecksum(const uint8* pucData, uint32 ulLen);
bool DataSendFrame(const DATA_FRAME* psFrame);
bool DataReceiveFrame(DATA_FRAME* psFrame, uint8* pucValueBuffer, uint32 ulMaxLen, uint32 ulTimeoutMs);

#endif // DATAFRAME_H
