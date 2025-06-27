//**************************** ledBlink ***************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : ledBlink.c
// Summary  : Simulates LED ON/OFF behavior using STM32 HAL
// Note     : None
// Author   : Anoop G
// Date     : 26/06/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "ledBlink.h"
#include "stm32f4xx_hal_gpio.h"

//*****************************.ledBlink.**************************************
// Purpose  : Toggles an LED on and off at fixed intervals using STM32 HAL.
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : Uses HAL_GPIO_WritePin() for GPIO control and osDelay() for
//            timing. Designed for STM32 boards with FreeRTOS support.
//*****************************************************************************
void ledBlink(void)
{
    /* Infinite loop */
    for(;;)
    {
	  HAL_GPIO_WritePin(LED_PB0_Pin_GPIO_Port, LED_PB0_Pin_Pin, GPIO_PIN_SET);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(LED_PB0_Pin_GPIO_Port, LED_PB0_Pin_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1000);
    }
}

//EOF


