//**************************** gpio *******************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : gpioWrite.c
// Summary  : Wrapper for HAL_GPIO_WritePin() to abstract STM32 HAL specifics
// Note     : None
// Author   : Anoop G
// Date     : 27/06/2025
//
//*****************************************************************************

//***************************** Include Files *********************************
#include "gpioWrite.h"
#include "stm32f4xx_hal_gpio.h" // Include the actual HAL GPIO header

//*****************************.GPIO_WritePin.*********************************
// Purpose  : Provides a wrapper around HAL_GPIO_WritePin() for abstracting
//            GPIO control.
// Inputs   : GPIOx    : Pointer to the GPIO port (e.g., GPIOB).
//            GPIO_Pin : The specific pin(s) to control (e.g., LED_PB0_Pin_Pin).
//            PinState : The desired state (GPIO_PIN_SET or GPIO_PIN_RESET).
// Outputs  : None
// Return   : None
// Notes    : This function simply calls the underlying STM32 HAL function.
//*****************************************************************************
void GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    // Call the STM32 HAL function directly
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

//EOF
