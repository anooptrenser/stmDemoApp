//**************************** ledBlink ***************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : ledBlink.c
// Summary  : Simulates LED ON/OFF behavior using GPIO wrapper
// Note     : None
// Author   : Anoop G
// Date     : 26/06/2025 (Modified: 27/06/2025)
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "ledBlink.h"        
#include "gpioWrite.h"       
#include "osDelay.h"

//*****************************.ledBlink.**************************************
// Purpose  : Toggles an LED on and off at fixed intervals using the GPIO 
//			  wrapper.
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : Uses GPIO_WritePin() for GPIO control and osDelay() for timing.
//*****************************************************************************
void ledBlink(void)
{
	  GpioWritePin(LED_PB0_Pin_GPIO_Port, LED_PB0_Pin_Pin, GPIO_PIN_SET);
	  osDelay(DELAY);
	  GpioWritePin(LED_PB0_Pin_GPIO_Port, LED_PB0_Pin_Pin, GPIO_PIN_RESET);
	  osDelay(DELAY);

}

//EOF
