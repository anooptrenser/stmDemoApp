//******************************* Parser **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : Parser.h
// Summary  : Serial frame parsing and extraction header
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
#ifndef PARSER_H
#define PARSER_H

//*********************Include Files*******************************************
#include <stdbool.h>
#include "UartProtoBuilder.h"

//************************** Function Declarations ****************************
void ParseHeader(const uint8* ucHeader, DATA_FRAME* psFrame);
bool ParseValidateChecksum(const uint8* pucValue, uint32 ulLength, 
					  uint8 ucReceivedChecksum);
bool ParsePayload(DATA_FRAME* pFrame, const uint8* pucPayload);

#endif // FRAME_PARSER_H

// EOF
