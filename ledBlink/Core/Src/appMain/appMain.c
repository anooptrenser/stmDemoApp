//**************************** appMain ****************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : appMain.c
// Summary  : Application main function to demonstrate ledBlink usage
// Note     : None
// Author   : Anoop G
// Date     : 26/06/2025
//
//*****************************************************************************
//***************************** Include Files *********************************
#include "ledBlink.h" // Include the ledBlink header

//*****************************.appMain.***************************************
// Purpose  : Wrapper for the ledBlink() routine
// Inputs   : None
// Outputs  : None
// Return   : None
// Notes    : Designed for use with STM32 HAL.
//*****************************************************************************
void appMain(void)
{
    // Call the ledBlink function
    ledBlink();
}
//EOF

