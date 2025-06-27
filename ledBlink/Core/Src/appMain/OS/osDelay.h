//**************************** osDelay ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : osDelay.h
// Summary  : Header file for OS-level delay wrapper functions
// Note     : None
// Author   : Anoop G
// Date     : 27/06/2025
//
//*****************************************************************************
#ifndef _OSDELAY_H_
#define _OSDELAY_H_

//***************************** Include Files *********************************
#include "main.h" 

//***************************** Global Constants ****************************** 
#define DELAY 1000

//***************************** Function Declarations *************************
void osDelay(uint32_t milliseconds);

#endif /* _OSDELAY_H_ */
//EOF
