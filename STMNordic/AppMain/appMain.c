//*******************************AppMain***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : AppMain.c
//Summary  : Application entry point
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include "AppMain.h"
#include "DataSender.h"

//*********************.AppMain.**********************************************
//Purpose : Entry point for the application
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void AppMain(void)
{
    DataSender(); // All protocol logic is inside DataSender()
}

//EOF
