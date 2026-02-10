/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM),  web: www.lcm.at
 * All rights reserved.
 */
/*
 * This file is licensed according to the BSD 3-clause license as follows:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the "Linz Center of Mechatronics GmbH" and "LCM" nor
 *       the names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL "Linz Center of Mechatronics GmbH" BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * This file is part of X2C. web: x2c.lcm.at
 * $LastChangedRevision: 3474 $
 * $LastChangedDate:: 2024-11-06 18:00:53 +0100#$
 */
#include <stddef.h>
#include "TableStruct.h"

/* init tablestruct */
void initTableStruct(void)
{
	uint8 i;

	TableStruct->TParamTable = NULL;
	TableStruct->TFncTable = NULL;
	TableStruct->TLimitSaveFncTable = NULL;

	TableStruct->DSPState = BOOTLOADER_STATE;
	TableStruct->eventType = (uint16)0x0000U;
	TableStruct->eventId = (uint32)0x00000000U;

	TableStruct->protocolCount = (uint16)0U;
	for (i = 0; i < MAX_PROTOCOLS; i++)
	{
		TableStruct->protocols[i] = NULL;
	}

	TableStruct->framePrgVersion = (uint16)0x0000U;
	TableStruct->framePrgCompDateTime = NULL;

	TableStruct->piScope = NULL;
	TableStruct->dynamicCodeData = NULL;

	TableStruct->inportParamTable = NULL;
	TableStruct->outportParamTable = NULL;
	TableStruct->maskParameterTable = NULL;
}


/* add tProtocol to TableStruct */
void addTableStructProtocol(tProtocol* protocol)
{
	if (TableStruct->protocolCount < MAX_PROTOCOLS)
	{
		TableStruct->protocols[TableStruct->protocolCount] = protocol;
		TableStruct->protocolCount++;
	}
}
