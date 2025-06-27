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
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

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
    bool blLedOn = false;
    /* Infinite loop */
    for(;;)
    {
        if (blLedOn)
        {
            printf("UART: LED ON\r\n");
            HAL_Delay(1000);
        } else
        {
        	printf("UART: LED OFF\r\n");
            HAL_Delay(1000);
        }

        blLedOn = !blLedOn;
    }
}

//EOF
