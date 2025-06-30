//**************************** GpioWrite **************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for GPIO wrapper functions.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************
//******************************* Include Files *******************************
#include "gpioWrite.h"

//******************************.GpioWrite.************************************
// Purpose : Sets the output state of a GPIO pin.
// Inputs  : pin   - Pin number
//           port  - GPIO port
//           state - Desired output state (0 or 1)
// Outputs : None
// Notes   : None
//*****************************************************************************
void GpioWrite(uint16_t pin, GPIO_TypeDef* port, uint8_t state)
{
    HAL_GPIO_WritePin(port, pin, (GPIO_PinState)state);
}

//******************************.GpioToggle.***********************************
// Purpose : Toggles the output state of a GPIO pin.
// Inputs  : pin  - Pin number
//           port - GPIO port
// Outputs : None
// Notes   : None
//*****************************************************************************
void GpioToggle(uint16_t pin, GPIO_TypeDef* port)
{
    HAL_GPIO_TogglePin(port, pin);
}

//******************************.GpioRead.*************************************
// Purpose : Reads the input state of a GPIO pin.
// Inputs  : pin  - Pin number
//           port - GPIO port
// Outputs : None
// Return  : uint8_t - Pin state (0 or 1)
// Notes   : None
//*****************************************************************************
uint8_t GpioRead(uint16_t pin, GPIO_TypeDef* port)
{
    return (uint8_t)HAL_GPIO_ReadPin(port, pin);
}

//EOF
