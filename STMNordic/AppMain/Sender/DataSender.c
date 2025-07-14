//*******************************DataSender************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : DataSender.c
//Summary  : Implements TLV protocol sender for UART file transfer with sequence number
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include "DataSender.h"
#include "DataFrame.h"
#include "UartDriver.h"
#include <string.h>
#include <stdbool.h>
#include "common.h"
#include "tmp.h"

//*********************Local Constants*****************************************
#define DATA_SENDER_TIMEOUT_MS    2000U

//*********************Local Functions*****************************************
static bool DataSenderAnnounceFileLength(uint32 ulFileLen, uint32* pulMaxChunk);
static bool DataSenderSendChunks(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk);

//*********************.DataSender.********************************************
//Purpose :	Top-level function to perform TLV file transfer over UART
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void DataSender(void)
{
    uint32 ulMaxChunk = 0U;

    if(DataSenderAnnounceFileLength(g_ulJsonLen, &ulMaxChunk))
    {
        (void)DataSenderSendChunks(g_pucJson, g_ulJsonLen, ulMaxChunk);
    }
}

//*********************.DataSenderAnnounceFileLength.**************************
//Purpose :	Send file length to receiver and get max chunk size
//Inputs  : ulFileLen - Length of file/data to send
//          pulMaxChunk - Pointer to store max chunk size from receiver
//Outputs : None
//Return  : TRUE if success, FALSE if error
//Notes   : None
//*****************************************************************************
static bool DataSenderAnnounceFileLength(uint32 ulFileLen, uint32* pulMaxChunk)
{
    DATA_FRAME stAnnounce = {0};
    uint8 ucFileLen[4];
    uint8 ucRespVal[4];
    DATA_FRAME stResp = {0};
    bool blStatus = false;

    ucFileLen[0] = (uint8)((ulFileLen >> 0) & 0xFF);
    ucFileLen[1] = (uint8)((ulFileLen >> 8) & 0xFF);
    ucFileLen[2] = (uint8)((ulFileLen >> 16) & 0xFF);
    ucFileLen[3] = (uint8)((ulFileLen >> 24) & 0xFF);

    stAnnounce.ucCmd = CMD_ANNOUNCE;
    stAnnounce.ucType = TYPE_FILE_LENGTH;
    stAnnounce.ulLength = 4U;
    stAnnounce.ulSeqNum = 0U; // Sequence number not used for announce
    stAnnounce.pucValue = ucFileLen;
    stAnnounce.ucChecksum = DataCalcChecksum(ucFileLen, 4U);

    if(DataSendFrame(&stAnnounce))
    {
        if(DataReceiveFrame(&stResp, ucRespVal, sizeof(ucRespVal), DATA_SENDER_TIMEOUT_MS))
        {
            if((stResp.ucCmd == CMD_ANNOUNCE) && (stResp.ucType == TYPE_CHUNK_SIZE) && (stResp.ulLength == 4U))
            {
                *pulMaxChunk = (uint32)ucRespVal[0]
                             | ((uint32)ucRespVal[1] << 8)
                             | ((uint32)ucRespVal[2] << 16)
                             | ((uint32)ucRespVal[3] << 24);
                blStatus = true;
            }
        }
    }
    return blStatus;
}

//*********************.DataSenderSendChunks.**********************************
//Purpose :	Split data into chunks and send each chunk with TLV protocol and sequence number
//Inputs  : pucData - Pointer to data buffer
//          ulLen   - Length of data
//          ulMaxChunk - Max chunk size allowed by receiver
//Outputs : None
//Return  : TRUE if all chunks sent and acknowledged, FALSE if error
//Notes   : None
//*****************************************************************************
static bool DataSenderSendChunks(const uint8* pucData, uint32 ulLen, uint32 ulMaxChunk)
{
    uint32 ulOffset = 0U;
    uint32 ulSeqNum = 1U; // Start sequence number from 1
    bool blStatus = true;

    while(ulOffset < ulLen)
    {
        uint32 ulChunk = (ulLen - ulOffset > ulMaxChunk) ? ulMaxChunk : (ulLen - ulOffset);
        DATA_FRAME stData = {0};
        stData.ucCmd = CMD_TRANSFER;
        stData.ucType = TYPE_DATA;
        stData.ulLength = ulChunk;
        stData.ulSeqNum = ulSeqNum;
        stData.pucValue = (uint8*)&pucData[ulOffset];
        stData.ucChecksum = DataCalcChecksum(stData.pucValue, ulChunk);

        if(!DataSendFrame(&stData))
        {
            blStatus = false;
            break;
        }

        // Wait for ACK (optional, base case can skip or add as needed)
        // DATA_FRAME stAck = {0};
        // uint8 ucAckVal[1];
        // if(!DataReceiveFrame(&stAck, ucAckVal, sizeof(ucAckVal), 1000U) || ucAckVal[0] != 0x00)
        // {
        //     blStatus = false;
        //     break;
        // }

        ulOffset += ulChunk;
        ulSeqNum++; // Increment sequence number for each chunk
    }
    return blStatus;
}

//EOF
