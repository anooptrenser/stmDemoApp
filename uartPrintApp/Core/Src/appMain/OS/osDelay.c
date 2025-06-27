//**************************** osDelay ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : osDelay.c
// Summary  : Implements an OS-level delay using HAL_Delay()
// Note     : None
// Author   : Anoop G
// Date     : 27/06/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "osDelay.h"
#include "stm32f4xx_hal.h" // Include the actual HAL header for HAL_Delay

//*****************************.osDelay.***************************************
// Purpose  : Provides an OS-level delay function that wraps HAL_Delay().
// Inputs   : milliseconds: The delay time length in milliseconds.
// Outputs  : None
// Return   : None
// Notes    : This function abstracts the specific HAL delay mechanism.
//*****************************************************************************
void osDelay(uint32_t milliseconds)
{
    // Call the STM32 HAL delay function
    HAL_Delay(milliseconds);
}
//EOF
