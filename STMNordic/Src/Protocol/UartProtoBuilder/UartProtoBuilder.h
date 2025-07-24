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
#define MAX_FRAME_SIZE  256
#define UART_START_BYTE 0xFF
#define UART_STOP_BYTE  0x0E
#define FRAME_HEADER_SIZE     8 // Cmd (1), Type (1), Length (4), SeqNum (2)
#define FRAME_BYTES_OVERHEAD  3 // Start, Checksum, Stop
#define MAX_RAW_FRAME_LEN   (MAX_FRAME_SIZE + FRAME_HEADER_SIZE + 3)

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
typedef struct
{
    uint8 ucStartByte;     
    uint8 ucCmd;
    uint8 ucType;
    uint32 ulLength;
    uint16 unSeqNum;
    uint8 *pucValue;      
    uint8 ucChecksum;
    uint8 ucStopByte;      
} DATA_FRAME;

//************************** Function Declarations ****************************
uint8 UartProtoCalcChecksum(const uint8* pucData, uint32 ulLen);
uint32 UartProtoBuildFrame(const DATA_FRAME* psFrame, uint8* ucbuffer, uint32 maxBufSize);
bool UartProtoSendFrame(const DATA_FRAME* psFrame);
bool UartProtoSendChunk(const uint16 unSeqNum, const uint8* pucData, const uint32 ulLength);

#endif

// EOF
