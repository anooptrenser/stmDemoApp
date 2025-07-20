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
bool ReceiveDataFrame(DATA_FRAME* psFrame, uint8* pucBuffer, uint32 ulMaxLen,
					  uint32 ulTimeoutMs);
bool ValidateChecksum(const uint8* pucValue, uint32 ulLength, 
					  uint8 ucReceivedChecksum);
bool ReceiveHeader(uint8* pucHeader, uint32 ulTimeoutMs);
bool ReceiveValueWithChecksum(uint8* pucBuffer, uint32 ulToReceive,
									 uint32 ulTimeoutMs);
bool ValidateAndExtractValue(DATA_FRAME* psFrame, const uint8* pucBuffer);
#endif // FRAME_PARSER_H

// EOF
