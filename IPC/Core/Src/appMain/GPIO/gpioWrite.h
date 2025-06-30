//**************************** GpioWrite ***************************************
//  Copyright (c) 2025 Trenser Technology Solutions
//  All Rights Reserved
//*****************************************************************************
//
//  Summary   : Header file for GPIO wrapper functions and macros.
//  Note      : Follows Trenser Embedded Coding Standard V1.0.
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

//******************************* Macros ***************************************
#define GPIO_LED_ON()        GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_SET)
#define GPIO_LED_OFF()       GpioWrite(LED_PIN, LED_PORT, GPIO_PIN_RESET)
#define GPIO_LED_TOGGLE()    GpioToggle(LED_PIN, LED_PORT)
#define GPIO_READ_BUTTON()   GpioRead(USER_BUTTON_PIN, USER_BUTTON_PORT)

//******************************* Function Prototypes **************************
void GpioInit(void);
void GpioWrite(uint16_t pin, GPIO_TypeDef* port, uint8_t state);
void GpioToggle(uint16_t pin, GPIO_TypeDef* port);
uint8_t GpioRead(uint16_t pin, GPIO_TypeDef* port);

#endif /* GPIO_WRITE_H */

//EOF
