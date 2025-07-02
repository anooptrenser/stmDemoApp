//**************************** GpioWrite **************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Source file for GPIO wrapper functions.
//  Note      : Provides abstraction for GPIO read/write using enums for
//              portability and clarity.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

//******************************* Include Files *******************************
#include "gpioWrite.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

//******************************* Local Functions *****************************

/*******************************************************************************
* Function Name : GetHalPort
* Description   : Maps the GPIO_PORT enum to the STM32 HAL GPIO_TypeDef pointer.
* Input(s)      : enPort - Enum value for the port.
* Output(s)     : None
* Return Value  : Pointer to GPIO_TypeDef, or NULL if invalid.
* Note(s)       : Local helper function.
*******************************************************************************/
static GPIO_TypeDef* GetHalPort(GPIO_PORT enPort)
{
    switch (enPort)
    {
        case GPIO_PORT_A: return GPIOA;
        case GPIO_PORT_B: return GPIOB;
        case GPIO_PORT_C: return GPIOC;
        case GPIO_PORT_D: return GPIOD;
        case GPIO_PORT_E: return GPIOE;
        default:
            printf("GpioWrite: Invalid port enum: %d\n", (int)enPort);
            return NULL;
    }
}

/*******************************************************************************
* Function Name : GetHalPin
* Description   : Converts LED_PIN enum to HAL pin mask.
* Input(s)      : enLedPin - Enum value for the pin (0 to 15).
* Output(s)     : None
* Return Value  : Pin mask for HAL functions, or 0 if invalid.
* Note(s)       : Local helper function.
*******************************************************************************/
static uint16_t usGetHalPin(LED_PIN enLedPin)
{
    if ((enLedPin >= LED_PIN_0) && (enLedPin <= LED_PIN_15))
    {
        return (uint16_t)(1U << enLedPin);
    }
    printf("GpioWrite: Invalid pin enum: %d\n", (int)enLedPin);
    return 0U;
}

//******************************* Global Functions ****************************

/*******************************************************************************
* Function Name : GpioWrite
* Description   : Sets the output state of a GPIO pin.
* Input(s)      : enPort   - GPIO port enum.
*                 enLedPin - LED pin enum.
*                 ucState  - Desired output state (0 for RESET, 1 for SET).
* Output(s)     : None
* Return Value  : None
* Note(s)       : Abstraction for portability; maps enums to HAL types.
*******************************************************************************/
void GpioWrite(GPIO_PORT enPort, LED_PIN enLedPin, uint8 ucState)
{
    GPIO_TypeDef* pstHalPort = GetHalPort(enPort);
    uint16_t usHalPin = usGetHalPin(enLedPin);

    if ((pstHalPort != NULL) && (usHalPin != 0U))
    {
        HAL_GPIO_WritePin(pstHalPort, usHalPin, (GPIO_PinState)ucState);
    }
    else
    {
        printf("GpioWrite: Invalid port or pin\n");
    }
}

/*******************************************************************************
* Function Name : GpioRead
* Description   : Reads the input state of a GPIO pin.
* Input(s)      : enPort   - GPIO port enum.
*                 enLedPin - LED pin enum.
* Output(s)     : None
* Return Value  : Pin state (0 or 1).
* Note(s)       : Abstraction for portability; maps enums to HAL types.
*******************************************************************************/
uint8 GpioRead(GPIO_PORT enPort, LED_PIN enLedPin)
{
    GPIO_TypeDef* pstHalPort = GetHalPort(enPort);
    uint16_t usHalPin = usGetHalPin(enLedPin);

    if ((pstHalPort != NULL) && (usHalPin != 0U))
    {
        return (uint8)HAL_GPIO_ReadPin(pstHalPort, usHalPin);
    }
    else
    {
        printf("GpioRead: Invalid port or pin\n");
        return 0U;
    }
}

//EOF
