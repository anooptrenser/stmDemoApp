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
#include "FileTransferManager.h"
#include <stdio.h>

//*********************.AppMain.**********************************************
//Purpose : Entry point for the application
//Inputs  : None
//Outputs : None
//Return  : None
//Notes   : None
//*****************************************************************************
void AppMain(void)
{
    if (!FileTransferManager())
       {
           printf("Error: DataSend failed\r\n");
           return;
       }
}

//EOF
