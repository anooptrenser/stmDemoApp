//**************************** gpio *******************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : gpio.h
// Summary  : Header file for GPIO wrapper functions
// Note     : None
// Author   : Anoop G
// Date     : 27/06/2025
//
//*****************************************************************************
#ifndef _GPIO_H_
#define _GPIO_H_

//***************************** Include Files *********************************
#include "main.h"

//***************************** Function Declarations *************************
void GpioWritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

#endif /* _GPIO_H_ */
//EOF
