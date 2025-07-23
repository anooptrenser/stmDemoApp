//*******************************UartDriver************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : UartDriver.c
//Summary  : UART abstraction layer implementation
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <common.h>
#include <stdio.h>
#include "UartDriver.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "CircularBuffer.h"
#include "Parser.h"
#include "OsFactory.h"
#include "OSQueue.h"

//***************************** Global Variables ******************************
// RTOS queue handle for frames
void *gFrameQueueHandle = NULL;
// UART RX single-byte buffer initialized
uint8 ucData = 0U;
// UART RX circular buffer initialized
UART_RX_BUFFER gUartRxBuffer = {0};

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initialize UART frame queue for inter-task communication
// Inputs  : None
// Outputs : None
// Return  : bool - true if queue created successfully, false otherwise
// Notes   :
//**********************************************************************************
bool InitUartFrameQueue(void)
{
    bool blResult = true;
    
    // Create queue for frames
    if (!OSQueueCreate(&gFrameQueueHandle, sizeof(DATA_FRAME), 8))
    {
        printf("[UART INIT] Failed to init UART frame queue\n");
        blResult = false;
    }

    return blResult;
}

//******************************.FUNCTION_HEADER.******************************
//Purpose : Send data over UART
//Inputs  : pucData - pointer to data buffer
//          ulLen   - number of bytes to send
//Outputs : None
//Return  : TRUE if successful, FALSE otherwise
//Notes   : None
//*****************************************************************************
bool UartSend(const uint8* pucData, uint32 ulLen)
{
    return (HAL_UART_Transmit(&huart2, (uint8*)pucData, ulLen, TIMEOUT) == HAL_OK);
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : Initialize UART interrupt-based reception using HAL API.
// Inputs  : pvUartHandler - Pointer to UART handler structure.
//           pucBuffer     - Pointer to data buffer for reception.
//           unBufferSize  - Size of the buffer in bytes.
// Outputs : NULL
// Return  : bool - true if interrupt reception was successfully started,
//                  false if initialization failed.
// Notes   : NULL
//**********************************************************************************
bool UartIntrInit(void *pvUartHandler, uint8 *pucBuffer, uint16 unBufferSize)
{
    bool bReturn = false;

    if ((pvUartHandler != NULL) && (pucBuffer != NULL))
    {
        if (!HAL_UART_Receive_IT(pvUartHandler, pucBuffer, unBufferSize))
        {
        	bReturn = true;
        }
    }

    return bReturn;
}

//******************************.FUNCTION_HEADER.******************************
// Purpose : UART RX complete callback, pushes received byte to circular buffer
// Inputs  : huart - UART handle pointer
// Outputs : None
// Return  : None
// Notes   : Called by HAL when a byte is received in interrupt mode
//**********************************************************************************
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        // Push received byte into circular buffer
        UartRxBufferPush(&gUartRxBuffer, ucData);

        // Re-enable UART interrupt for next byte
        UartIntrInit(&huart2, &ucData, 1);
    }
}


