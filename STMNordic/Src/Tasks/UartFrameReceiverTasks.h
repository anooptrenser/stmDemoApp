//******************************* UartFrameReceiver **************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : UartFrameReceiver.h
// Summary  : UART frame extraction thread prototype
// Author   : Anoop G
// Date     : 22-07-2025
//
//*****************************************************************************
#ifndef UART_FRAME_RECEIVER_H
#define UART_FRAME_RECEIVER_H

//***************************** Global Constants ******************************
#define UART_FRAME_RECEIVER_TASK_STACK_SIZE		5120
#define UART_FRAME_RECEIVER_TASK_NAME		    "UartFrameReceiverTask"

//**************************** Forward Declarations ***************************
void UartFrameReceiverTask(void *pvArgs);

#endif // UART_FRAME_RECEIVER_H
