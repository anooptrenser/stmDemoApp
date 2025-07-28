//**************************** UartEventHandler *******************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//*****************************************************************************
//
// File     : UartEventHandler.c
// Summary  : UART RX buffer, event handlers, and frame extraction for nRF52
// Author   : Anoop
// Date     : 20-07-2025
//*****************************************************************************
//******************************* Include Files ******************************
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "FileTransfer.h"
#include "UartProtoBuilder.h"
#include "Parser.h"
#include "common.h"
#include "CircularBuffer.h"
#include "AppMain.h"
#include <stdlib.h>
#include "nrf_delay.h"
#include <ctype.h>
#include "UartFrameHandler.h"

//******************************* Local Types *********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//****************************** Local Functions ******************************

//******************************.UartInit.*************************************
// Purpose  : Initializes the UART peripheral for BLE-UART bridge communication.
// Inputs   : None
// Outputs  : None
// Returns  : bool - TRUE if initialization is successful, FALSE otherwise.
//*****************************************************************************
bool UartInit(void)
{
    UART_COMM_PARAMS sUartParams = {0};
    uint32 ulErrCode = NRF_SUCCESS;

    sUartParams.rx_pin_no    = UART_RX_PIN_NUMBER;
    sUartParams.tx_pin_no    = UART_TX_PIN_NUMBER;
    sUartParams.rts_pin_no   = UART_RTS_PIN_NUMBER;
    sUartParams.cts_pin_no   = UART_CTS_PIN_NUMBER;
    sUartParams.flow_control = APP_UART_FLOW_CONTROL_DISABLED;
    sUartParams.use_parity   = false;
    sUartParams.baud_rate    = UART_BAUDRATE;

    ulErrCode = UartApiFifoInit(&sUartParams, UartEventHandle, APP_IRQ_PRIORITY_LOWEST);

    if (ulErrCode != NRF_SUCCESS)
    {
        printf("UartInit: UartApiFifoInit failed, error: 0x%08X", ulErrCode);
        return false;
    }

    return true;
}

//******************************.UartHandleError.******************************
// Purpose  : Handles UART communication and FIFO errors detected in the event.
// Inputs   : psEvent - Pointer to UART_EVENT containing error details
// Outputs  : None
// Returns  : None
//*****************************************************************************
void UartHandleError(UART_EVENT *psEvent)
{
    if (psEvent->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        printf("UART communication error: 0x%08X", psEvent->data.error_communication);
        APP_ERROR_HANDLER(psEvent->data.error_communication);
    }
    else if (psEvent->evt_type == APP_UART_FIFO_ERROR)
    {
        printf("UART FIFO error: 0x%08X", psEvent->data.error_code);
        APP_ERROR_HANDLER(psEvent->data.error_code);
    }
}

//******************************.UartEventHandle.******************************
// Purpose  : Central UART event dispatcher that routes data and error events
//            to corresponding handler functions.
// Inputs   : psEvent - Pointer to UART_EVENT structure
// Outputs  : None
// Returns  : None
//*****************************************************************************
void UartEventHandle(UART_EVENT *psEvent)
{
    if (psEvent->evt_type == APP_UART_DATA_READY)
    {
        uint8_t ucByte = 0;

        if (UartApiGet(&ucByte) == NRF_SUCCESS)
        {
            UartRxBufferPush(&gUartRxBuffer, ucByte);
        }
    }
    else if (psEvent->evt_type == APP_UART_FIFO_ERROR)
    {
        UartHandleError(psEvent);
    }
}

// EOF
