//**************************** OsQueue ****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for OS queue wrapper functions and message types.
//  Note      : None
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef OSQUEUE_H
#define OSQUEUE_H

//******************************* Include Files *******************************
#include <stdbool.h>
#include "cmsis_os2.h"
#include "common.h"

//******************************* Constants ***********************************
#define POLLER_TO_RECEIVER_QUEUE_SIZE    (16)
#define RECEIVER_TO_POLLER_QUEUE_SIZE    (16)

//******************************* Global Types ********************************
typedef enum
{
    STATE_OK    = 0x00,
    STATE_ERROR = 0x01
} STATE_TYPE;

typedef enum
{
    CMD_ACK   = 0x00,
    CMD_GET   = 0x01,
    CMD_SET   = 0x02,
    CMD_CLEAR = 0x03
} COMMAND_TYPE;

typedef struct
{
    uint32 ulUid;
    uint8  ucCmd;
    uint32 ulData;
} REQUEST_MESSAGE;

typedef struct
{
    uint32 ulUid;
    uint8  ucCmd;
    uint8  ucState;
    uint32 ulData;
} ACK_MESSAGE;

//******************************* Global Variables ****************************
extern osMessageQueueId_t pPollerToReceiverQueueHandle;
extern osMessageQueueId_t pReceiverToPollerQueueHandle;

//******************************* Function Prototypes *************************
bool OsQueueSendRequest(REQUEST_MESSAGE* pstRequestMessage);
bool OsQueueReceiveRequest(REQUEST_MESSAGE* pstRequestMessage, uint32 ulTimeout);
bool OsQueueSendAck(ACK_MESSAGE* pstAckMessage);
bool OsQueueReceiveAck(ACK_MESSAGE* pstAckMessage, uint32 ulTimeout);

#endif /* OSQUEUE_H */

//EOF
