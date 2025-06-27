//**************************** appMain ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : appMain.c
// Summary  : Application main function to demonstrate uartPrint usage
// Note     : None
// Author   : Anoop G
// Date     : 26/06/2025
//
//*****************************************************************************
//***************************** Include Files *********************************
#include "uartPrint.h" // Include the uartPrint header

//*****************************.appMain.***************************************
// Purpose  : Wrapper for the uartPrint() routine
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : Designed for use with STM32 HAL.
//*****************************************************************************
void appMain(void)
{
    // Call the uartPrint function
    uartPrint();
}
//EOF

