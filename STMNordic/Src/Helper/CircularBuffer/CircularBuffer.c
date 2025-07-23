//**************************** Helper ******************************************
//  Copyright (c) 2021 Trenser
//  All Rights Reserved
//******************************************************************************
//
//  File      : CircularBuffer.c
//  Summary   : Implementation of UART Circular RX Buffer
//  Author    : Anoop G
//  Date      : 20-07-2025
//
//******************************************************************************
//******************************* Include Files ********************************
#include "CircularBuffer.h"
#include <stddef.h>
//******************************.FUNCTION_HEADER.******************************
// Purpose : Initialize UART RX buffer by resetting head and tail to 0.
// Inputs  : pRxBuffer - Pointer to UART RX buffer object.
// Outputs : None
// Return  : None
// Notes   : None
//**********************************************************************************
bool UartRxBufferInit(UART_RX_BUFFER* pRxBuffer)
{
    bool blResult = false;

    if (pRxBuffer != NULL)
    {
        pRxBuffer->unHead = 0U;
        pRxBuffer->unTail = 0U;
        blResult = true;
    }
    
    return blResult;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Insert one byte into the UART RX circular buffer if space exists.
// Inputs  : pRxBuffer - Pointer to UART RX buffer object.
//           ucData    - Data byte to insert.
// Outputs : None
// Return  : bool - true if data is inserted, false if buffer is full.
// Notes   : None
//**********************************************************************************
bool UartRxBufferPush(UART_RX_BUFFER* pRxBuffer, uint8 ucData)
{
    bool blResult = false;
    if (pRxBuffer != NULL)
    {
        uint16 unNext = (pRxBuffer->unHead + 1U) % UART_RX_BUF_SIZE;

        if (unNext != pRxBuffer->unTail)
        {
            pRxBuffer->pucBuffer[pRxBuffer->unHead] = ucData;
            pRxBuffer->unHead = unNext;
            blResult = true;
        }
    }

    return blResult;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Retrieve one byte from UART RX buffer if available.
// Inputs  : pRxBuffer - Pointer to UART RX buffer object.
// Outputs : pucData  - Pointer to store the popped byte.
// Return  : bool - true if data was present and popped, false if buffer is empty.
// Notes   : None
//**********************************************************************************
bool UartRxBufferPop(UART_RX_BUFFER* pRxBuffer, uint8* pucData)
{
    bool blResult = false;

    if ((pRxBuffer != NULL) && (pucData != NULL))
    {
        if (pRxBuffer->unHead != pRxBuffer->unTail)
        {
            *pucData = pRxBuffer->pucBuffer[pRxBuffer->unTail];
            pRxBuffer->unTail = (pRxBuffer->unTail + 1U) % UART_RX_BUF_SIZE;
            blResult = true;
        }
    }

    return blResult;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Count number of bytes currently stored in the RX buffer.
// Inputs  : pRxBuffer - Pointer to UART RX buffer object.
// Outputs : None
// Return  : uint16 - Number of bytes in buffer.
// Notes   : None
//**********************************************************************************
uint16 UartRxBufferCount(UART_RX_BUFFER* pRxBuffer)
{
    uint16 unCount = 0U;

    if (pRxBuffer != NULL)
    {
        if (pRxBuffer->unHead >= pRxBuffer->unTail)
        {
            unCount = pRxBuffer->unHead - pRxBuffer->unTail;
        }
        else
        {
            unCount = UART_RX_BUF_SIZE - pRxBuffer->unTail + pRxBuffer->unHead;
        }
    }

    return unCount;
}

//EOF
