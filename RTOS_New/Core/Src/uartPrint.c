//**************************** uartPrint **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : uartPrint.c
// Summary  : Continuously toggles the LED state message and transmits it via
//            UART using the uartPrintMessage() function.
// Note     : None
// Author   : Anoop G
// Date     : 26/06/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "uartPrint.h"
#include "cmsis_os.h" // Include FreeRTOS header
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

//***************************** Global Variables ******************************
extern UART_HandleTypeDef huart3; // Ensure UART handle is declared

//*****************************.uartPrintMessage.******************************
// Purpose  : Transmits a string message over UART using STM32 HAL.
// Inputs   : message : Pointer to the null-terminated string to be sent.
// Outputs  : None
// Return   : None
// Notes    :  Uses the global UART handle 'huart3'. 
//*****************************************************************************
void uartPrintMessage(const char* message)
{
      HAL_UART_Transmit(&huart3, (uint8_t *)message, strlen(message),
                        HAL_MAX_DELAY);
}

//*****************************.uartPrint.*************************************
// Purpose  : Alternately prints "UART: LED ON" and "UART: LED OFF" messages
//            over UART.
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : Designed for use with STM32 HAL and FreeRTOS. The function 
//            runs in an infinite loop and should be used as a task or thread.          
//*****************************************************************************
void uartPrint(void)
{
    bool tblLedOn = false;
    /* Infinite loop */
    for(;;)
    {
        if (tblLedOn) {
            uartPrintMessage("UART: LED ON\r\n");
            osDelay(1000);
        } else {
            uartPrintMessage("UART: LED OFF\r\n");
            osDelay(1000);
        }

        tblLedOn = !tblLedOn;
    }
}

//EOF
