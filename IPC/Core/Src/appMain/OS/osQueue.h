//**************************** OsQueue ****************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for OS queue wrapper functions and message types.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef OS_QUEUE_H
#define OS_QUEUE_H

//******************************* Include Files *******************************
#include "cmsis_os2.h"

//******************************* Constants ***********************************
#define POLLER_TO_RECEIVER_QUEUE_SIZE     (16)
#define RECEIVER_TO_POLLER_QUEUE_SIZE     (16)

//******************************* Global Types ********************************
typedef enum
{
    STATE_OK = 0x00,
    STATE_ERROR = 0x01
} StateType;

typedef enum
{
    CMD_ACK = 0x00,
    CMD_GET = 0x01,
    CMD_SET = 0x02,
    CMD_CLEAR = 0x03
} CommandType;

typedef struct
{
    uint32_t uid;
    CommandType cmd;
    uint32_t data;
} RequestMessage;

typedef struct
{
    uint32_t uid;
    CommandType cmd;
    StateType state;
    uint32_t data;
} AckMessage;

//******************************* Global Variables ****************************
extern osMessageQueueId_t PollerToReceiverQueueHandle;
extern osMessageQueueId_t ReceiverToPollerQueueHandle;

//******************************* Function Prototypes *************************
osStatus_t OsQueueSendRequest(RequestMessage *msg);
osStatus_t OsQueueReceiveRequest(RequestMessage *msg, uint32_t timeout);
osStatus_t OsQueueSendAck(AckMessage *msg);
osStatus_t OsQueueReceiveAck(AckMessage *msg, uint32_t timeout);

#endif /* OS_QUEUE_H */

//EOF
