//*******************************tmp***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : Tmp.h
//Summary  : Sample data
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************

//*********************Include Files*******************************************
#include <stdio.h>
#include "common.h"

//*********************Local Variables*****************************************
extern const uint8 g_ucData[];
extern const uint32 g_ulDataLen;

//******************************* Global Types ********************************

//***************************** Global Constants ******************************
#define HEXDUMP_BYTES_PER_LINE   (16)
#define ENABLE_HEXDUMP           1

//***************************** Global Variables ******************************

//************************** Function Declarations ****************************
void HexDump(const void *pvData, uint32 ulSize);

//EOF
