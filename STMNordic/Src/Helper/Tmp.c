//*******************************tmp***************************************
//Copyright (c) 2025 Trenser Technology Solutions
//All Rights Reserved
//*****************************************************************************
//
//File     : Tmp.c
//Summary  : Sample data
//Note     : None
//Author   : Anoop G
//Date     : 14-07-2025
//
//*****************************************************************************
//*********************Include Files*******************************************
#include <stdio.h>
#include <string.h>
#include "common.h"
#include <ctype.h>
#include "Tmp.h"

//*********************Local Variables*****************************************
const uint8 g_ucData[] = "{"
		"\"sensor1\":{\"temp\":23,\"hum\":60,\"status\":\"ok\"},"
		"\"sensor2\":{\"temp\":24,\"hum\":61,\"status\":\"ok\"},"
		"\"sensor3\":{\"temp\":25,\"hum\":62,\"status\":\"ok\"},"
		"\"sensor4\":{\"temp\":26,\"hum\":63,\"status\":\"ok\"},"
		"\"sensor5\":{\"temp\":27,\"hum\":64,\"status\":\"ok\"},"
		"\"sensor6\":{\"temp\":28,\"hum\":65,\"status\":\"ok\"},"
		"\"sensor7\":{\"temp\":29,\"hum\":66,\"status\":\"ok\"},"
		"\"sensor8\":{\"temp\":30,\"hum\":67,\"status\":\"ok\"},"
		"\"sensor9\":{\"temp\":31,\"hum\":68,\"status\":\"ok\"},"
		"\"sensor10\":{\"temp\":32,\"hum\":69,\"status\":\"ok\"},"
		"\"sensor11\":{\"temp\":33,\"hum\":70,\"status\":\"ok\"},"
		"\"sensor12\":{\"temp\":34,\"hum\":71,\"status\":\"ok\"},"
		"\"sensor13\":{\"temp\":35,\"hum\":72,\"status\":\"ok\"},"
		"\"sensor14\":{\"temp\":36,\"hum\":73,\"status\":\"ok\"},"
		"\"sensor15\":{\"temp\":37,\"hum\":74,\"status\":\"ok\"},"
		"\"sensor16\":{\"temp\":38,\"hum\":75,\"status\":\"ok\"},"
		"\"sensor17\":{\"temp\":39,\"hum\":76,\"status\":\"ok\"},"
		"\"sensor18\":{\"temp\":40,\"hum\":77,\"status\":\"ok\"},"
		"\"sensor19\":{\"temp\":41,\"hum\":78,\"status\":\"ok\"},"
		"\"sensor20\":{\"temp\":42,\"hum\":79,\"status\":\"ok\"},"
		"\"sensor21\":{\"temp\":43,\"hum\":80,\"status\":\"ok\"},"
		"\"sensor22\":{\"temp\":44,\"hum\":81,\"status\":\"ok\"},"
		"\"sensor23\":{\"temp\":45,\"hum\":82,\"status\":\"ok\"},"
		"\"sensor24\":{\"temp\":46,\"hum\":83,\"status\":\"ok\"},"
		"\"sensor25\":{\"temp\":47,\"hum\":84,\"status\":\"ok\"},"
		"\"sensor26\":{\"temp\":48,\"hum\":85,\"status\":\"ok\"},"
		"\"sensor27\":{\"temp\":49,\"hum\":86,\"status\":\"ok\"},"
		"\"sensor28\":{\"temp\":50,\"hum\":87,\"status\":\"ok\"},"
		"\"sensor29\":{\"temp\":51,\"hum\":88,\"status\":\"ok\"},"
		"\"sensor30\":{\"temp\":52,\"hum\":89,\"status\":\"ok\"},"
		"\"sensor31\":{\"temp\":53,\"hum\":90,\"status\":\"ok\"},"
		"\"sensor32\":{\"temp\":54,\"hum\":91,\"status\":\"ok\"},"
		"\"sensor33\":{\"temp\":55,\"hum\":92,\"status\":\"ok\"},"
		"\"sensor34\":{\"temp\":56,\"hum\":93,\"status\":\"ok\"},"
		"\"sensor35\":{\"temp\":57,\"hum\":94,\"status\":\"ok\"},"
		"\"sensor36\":{\"temp\":58,\"hum\":95,\"status\":\"ok\"},"
		"\"sensor37\":{\"temp\":59,\"hum\":96,\"status\":\"ok\"},"
		"\"sensor38\":{\"temp\":60,\"hum\":97,\"status\":\"ok\"},"
		"\"sensor39\":{\"temp\":61,\"hum\":98,\"status\":\"ok\"},"
		"\"sensor40\":{\"temp\":62,\"hum\":99,\"status\":\"ok\"},"
		"\"sensor41\":{\"temp\":63,\"hum\":100,\"status\":\"ok\"},"
		"\"sensor42\":{\"temp\":64,\"hum\":101,\"status\":\"ok\"},"
		"\"sensor43\":{\"temp\":65,\"hum\":102,\"status\":\"ok\"},"
		"\"sensor44\":{\"temp\":66,\"hum\":103,\"status\":\"ok\"},"
		"\"sensor45\":{\"temp\":67,\"hum\":104,\"status\":\"ok\"},"
		"\"sensor46\":{\"temp\":68,\"hum\":105,\"status\":\"ok\"},"
		"\"sensor47\":{\"temp\":69,\"hum\":106,\"status\":\"ok\"},"
		"\"sensor48\":{\"temp\":70,\"hum\":107,\"status\":\"ok\"},"
		"\"sensor49\":{\"temp\":71,\"hum\":108,\"status\":\"ok\"},"
		"\"sensor50\":{\"temp\":72,\"hum\":109,\"status\":\"ok\"},"
		"\"sensor51\":{\"temp\":73,\"hum\":110,\"status\":\"ok\"},"
		"\"sensor52\":{\"temp\":74,\"hum\":111,\"status\":\"ok\"},"
		"\"sensor53\":{\"temp\":75,\"hum\":112,\"status\":\"ok\"},"
		"\"sensor54\":{\"temp\":76,\"hum\":113,\"status\":\"ok\"},"
		"\"sensor55\":{\"temp\":77,\"hum\":114,\"status\":\"ok\"},"
		"\"sensor56\":{\"temp\":78,\"hum\":115,\"status\":\"ok\"},"
		"\"sensor57\":{\"temp\":79,\"hum\":116,\"status\":\"ok\"},"
		"\"sensor58\":{\"temp\":80,\"hum\":117,\"status\":\"ok\"},"
		"\"sensor59\":{\"temp\":81,\"hum\":118,\"status\":\"ok\"},"
		"\"sensor60\":{\"temp\":82,\"hum\":119,\"status\":\"ok\"},"
		"\"sensor61\":{\"temp\":83,\"hum\":120,\"status\":\"ok\"},"
		"\"sensor62\":{\"temp\":84,\"hum\":121,\"status\":\"ok\"},"
		"\"sensor63\":{\"temp\":85,\"hum\":122,\"status\":\"ok\"},"
		"\"sensor64\":{\"temp\":86,\"hum\":123,\"status\":\"ok\"},"
		"\"sensor65\":{\"temp\":87,\"hum\":124,\"status\":\"ok\"},"
		"\"sensor66\":{\"temp\":88,\"hum\":125,\"status\":\"ok\"},"
		"\"sensor67\":{\"temp\":89,\"hum\":126,\"status\":\"ok\"},"
		"\"sensor68\":{\"temp\":90,\"hum\":127,\"status\":\"ok\"},"
		"\"sensor69\":{\"temp\":91,\"hum\":128,\"status\":\"ok\"},"
		"\"sensor70\":{\"temp\":92,\"hum\":129,\"status\":\"ok\"},"
		"\"sensor71\":{\"temp\":93,\"hum\":130,\"status\":\"ok\"},"
		"\"sensor72\":{\"temp\":94,\"hum\":131,\"status\":\"ok\"},"
		"\"sensor73\":{\"temp\":95,\"hum\":132,\"status\":\"ok\"},"
		"\"sensor74\":{\"temp\":96,\"hum\":133,\"status\":\"ok\"},"
		"\"sensor75\":{\"temp\":97,\"hum\":134,\"status\":\"ok\"},"
		"\"sensor76\":{\"temp\":98,\"hum\":135,\"status\":\"ok\"},"
		"\"sensor77\":{\"temp\":99,\"hum\":136,\"status\":\"ok\"},"
		"\"sensor78\":{\"temp\":100,\"hum\":137,\"status\":\"ok\"},"
		"\"sensor79\":{\"temp\":101,\"hum\":138,\"status\":\"ok\"},"
		"\"sensor80\":{\"temp\":102,\"hum\":139,\"status\":\"ok\"},"
		"\"sensor81\":{\"temp\":103,\"hum\":140,\"status\":\"ok\"},"
		"\"sensor82\":{\"temp\":104,\"hum\":141,\"status\":\"ok\"},"
		"\"sensor83\":{\"temp\":105,\"hum\":142,\"status\":\"ok\"},"
		"\"sensor84\":{\"temp\":106,\"hum\":143,\"status\":\"ok\"},"
		"\"sensor85\":{\"temp\":107,\"hum\":144,\"status\":\"ok\"},"
		"\"sensor86\":{\"temp\":108,\"hum\":145,\"status\":\"ok\"},"
		"\"sensor87\":{\"temp\":109,\"hum\":146,\"status\":\"ok\"},"
		"\"sensor88\":{\"temp\":110,\"hum\":147,\"status\":\"ok\"},"
		"\"sensor89\":{\"temp\":111,\"hum\":148,\"status\":\"ok\"},"
		"\"sensor90\":{\"temp\":112,\"hum\":149,\"status\":\"ok\"},"
		"\"sensor91\":{\"temp\":113,\"hum\":150,\"status\":\"ok\"},"
		"\"sensor92\":{\"temp\":114,\"hum\":151,\"status\":\"ok\"},"
		"\"sensor93\":{\"temp\":115,\"hum\":152,\"status\":\"ok\"},"
		"\"sensor94\":{\"temp\":116,\"hum\":153,\"status\":\"ok\"},"
		"\"sensor95\":{\"temp\":117,\"hum\":154,\"status\":\"ok\"},"
		"\"sensor96\":{\"temp\":118,\"hum\":155,\"status\":\"ok\"},"
		"\"sensor97\":{\"temp\":119,\"hum\":156,\"status\":\"ok\"},"
		"\"sensor98\":{\"temp\":120,\"hum\":157,\"status\":\"ok\"},"
		"\"sensor99\":{\"temp\":121,\"hum\":158,\"status\":\"ok\"},"
		"\"sensor100\":{\"temp\":122,\"hum\":159,\"status\":\"ok\"},"
		"\"sensor101\":{\"temp\":123,\"hum\":160,\"status\":\"ok\"},"
		"\"sensor102\":{\"temp\":124,\"hum\":161,\"status\":\"ok\"},"
		"\"sensor103\":{\"temp\":125,\"hum\":162,\"status\":\"ok\"},"
		"\"sensor104\":{\"temp\":126,\"hum\":163,\"status\":\"ok\"},"
		"\"sensor105\":{\"temp\":127,\"hum\":164,\"status\":\"ok\"},"
		"\"sensor106\":{\"temp\":128,\"hum\":165,\"status\":\"ok\"},"
		"\"sensor107\":{\"temp\":129,\"hum\":166,\"status\":\"ok\"},"
		"\"sensor108\":{\"temp\":130,\"hum\":167,\"status\":\"ok\"},"
		"\"sensor109\":{\"temp\":131,\"hum\":168,\"status\":\"ok\"},"
		"\"sensor110\":{\"temp\":132,\"hum\":169,\"status\":\"ok\"},"
		"\"sensor111\":{\"temp\":133,\"hum\":170,\"status\":\"ok\"},"
		"\"sensor112\":{\"temp\":134,\"hum\":171,\"status\":\"ok\"},"
		"\"sensor113\":{\"temp\":135,\"hum\":172,\"status\":\"ok\"},"
		"\"sensor114\":{\"temp\":136,\"hum\":173,\"status\":\"ok\"},"
		"\"sensor115\":{\"temp\":137,\"hum\":174,\"status\":\"ok\"},"
		"\"sensor116\":{\"temp\":138,\"hum\":175,\"status\":\"ok\"},"
		"\"sensor117\":{\"temp\":139,\"hum\":176,\"status\":\"ok\"},"
		"\"sensor118\":{\"temp\":140,\"hum\":177,\"status\":\"ok\"},"
		"\"sensor119\":{\"temp\":141,\"hum\":178,\"status\":\"ok\"},"
		"\"sensor120\":{\"temp\":142,\"hum\":179,\"status\":\"ok\"},"
		"\"sensor121\":{\"temp\":143,\"hum\":180,\"status\":\"ok\"},"
		"\"sensor122\":{\"temp\":144,\"hum\":181,\"status\":\"ok\"},"
		"\"sensor123\":{\"temp\":145,\"hum\":182,\"status\":\"ok\"},"
		"\"sensor124\":{\"temp\":146,\"hum\":183,\"status\":\"ok\"},"
		"\"sensor125\":{\"temp\":147,\"hum\":184,\"status\":\"ok\"},"
		"\"sensor126\":{\"temp\":148,\"hum\":185,\"status\":\"ok\"},"
		"\"sensor127\":{\"temp\":149,\"hum\":186,\"status\":\"ok\"},"
		"\"sensor128\":{\"temp\":150,\"hum\":187,\"status\":\"ok\"},"
		"\"sensor129\":{\"temp\":151,\"hum\":188,\"status\":\"ok\"},"
		"\"sensor130\":{\"temp\":152,\"hum\":189,\"status\":\"ok\"}"
		"}";


const uint32 g_ulDataLen = sizeof(g_ucData) - 1;

//******************************.FUNCTION_HEADER.******************************
// Purpose : Print a formatted hexadecimal and ASCII dump of binary data
// Inputs  : pvData - pointer to binary data buffer
//           ulSize - number of bytes to dump
// Outputs : None
// Return  : None
// Notes   : Useful for UART/debug prints. Uses serial-friendly line endings.
//*****************************************************************************
void HexDump(const void *pvData, uint32 ulSize)
{
	const uint8 *pucByte = (const uint8 *)pvData;
	uint32 ulLineOffset = 0;
	uint32 ulByteIdx = 0;
	char cChar = 0;

    for (ulLineOffset = 0; ulLineOffset < ulSize; ulLineOffset += HEXDUMP_BYTES_PER_LINE)
    {
        /* Print offset */
        printf("%08lx  ", (unsigned long)ulLineOffset);

        /* Print hex byte values */
        for (ulByteIdx = 0; ulByteIdx < HEXDUMP_BYTES_PER_LINE; ulByteIdx++)
        {
            if ((ulLineOffset + ulByteIdx) < ulSize)
            {
                printf("%02x ", pucByte[ulLineOffset + ulByteIdx]);
            }
            else
            {
                printf("   "); /* Padding for short lines */
            }

            if (ulByteIdx == 7U)
            {
                printf(" "); /* Extra space after 8 bytes */
            }
        }

        /* Print ASCII characters */
        printf(" |");
        for (ulByteIdx = 0; ulByteIdx < HEXDUMP_BYTES_PER_LINE; ulByteIdx++)
        {
            if ((ulLineOffset + ulByteIdx) < ulSize)
            {
                cChar = (char)pucByte[ulLineOffset + ulByteIdx];
                printf("%c", isprint((unsigned char)cChar) ? cChar : '.');
            }
        }
        printf("|\r\n");
    }

    fflush(stdout); /* Force flush in case of UART redirection */
}



//EOF
