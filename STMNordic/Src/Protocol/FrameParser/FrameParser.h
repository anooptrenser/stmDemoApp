//******************************* FrameParser *********************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// File     : FrameParser.h
// Summary  : Serial frame parsing and extraction header
// Author   : Anoop
// Date     : 15-07-2025
//
//*****************************************************************************
#ifndef FRAME_PARSER_H
#define FRAME_PARSER_H

//*********************Include Files*******************************************
#include "FrameBuilder.h"
#include <stdbool.h>

//************************** Function Declarations ****************************
void ParseHeader(const uint8* ucHeader, DATA_FRAME* psFrame);
bool DataReceiveFrame(DATA_FRAME* psFrame, uint8* pucBuffer, uint32 ulMaxLen, uint32 ulTimeoutMs);

#endif // FRAME_PARSER_H

// EOF
