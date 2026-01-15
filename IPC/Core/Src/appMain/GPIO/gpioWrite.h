//**************************** GpioWrite **************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for GPIO wrapper functions and macros.
//  Note      : Provides abstraction for GPIO read/write using enums for
//              portability and clarity.
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef GPIO_WRITE_H
#define GPIO_WRITE_H

//******************************* Include Files *******************************
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "common.h"  

//******************************* Macro Definitions ***************************
#define BUTTON_PRESSED_STATE    (0u)
#define BUTTON_RELEASED_STATE   (1u)

//******************************* Type Definitions ****************************
typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B,
    GPIO_PORT_C,
    GPIO_PORT_D,
    GPIO_PORT_E,
    GPIO_PORT_INVALID
} GPIO_PORT;

typedef enum
{
    LED_PIN_0 = 0,
    LED_PIN_1,
    LED_PIN_2,
    LED_PIN_3,
    LED_PIN_4,
    LED_PIN_5,
    LED_PIN_6,
    LED_PIN_7,
    LED_PIN_8,
    LED_PIN_9,
    LED_PIN_10,
    LED_PIN_11,
    LED_PIN_12,
    LED_PIN_13,
    LED_PIN_14,
    LED_PIN_15,
    LED_PIN_INVALID
} LED_PIN;

//******************************* Function Prototypes *************************
bool GpioWrite(GPIO_PORT enPort, LED_PIN enLedPin, uint8 ucState);
uint8 GpioRead(GPIO_PORT enPort, LED_PIN enLedPin);


#endif /* GPIO_WRITE_H */

//EOF
