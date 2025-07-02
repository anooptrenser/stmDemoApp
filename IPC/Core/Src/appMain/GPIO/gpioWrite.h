//**************************** GpioWrite ***************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for GPIO wrapper functions and macros.
//  Note      :
//  Author    : Anoop G
//  Date      : 27/06/2025
//
//*****************************************************************************

#ifndef GPIO_WRITE_H
#define GPIO_WRITE_H

//******************************* Include Files ********************************
#include "stm32f4xx_hal.h"

//******************************* Constants ************************************
#define USER_BUTTON_PIN      GPIO_PIN_13
#define USER_BUTTON_PORT     GPIOC
#define LED_PIN              GPIO_PIN_0
#define LED_PORT             GPIOB

#define BUTTON_PRESSED_STATE    (0u)
#define BUTTON_RELEASED_STATE   (1u)

//******************************* Function Prototypes **************************
void GpioInit(void);
void GpioWrite(uint16_t pin, GPIO_TypeDef* port, uint8_t state);
//void GpioToggle(uint16_t pin, GPIO_TypeDef* port);
uint8_t GpioRead(uint16_t pin, GPIO_TypeDef* port);

#endif /* GPIO_WRITE_H */

//EOF
