/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
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
/* USERCODE-BEGIN:Description                                                                                         */
/* Description:     Makes the incoming signal accessible for reading with parameter numbers.                          */
/*                                                                                                                    */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "SaveSignal_Float64.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** No Update function                                                                                               **/
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void SaveSignal_Float64_Init(SAVESIGNAL_FLOAT64 *block)
{
    block->ID = SAVESIGNAL_FLOAT64_ID;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SaveSignal_Float64_Load(const SAVESIGNAL_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    {
        *dataLength = 0U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
    if (maxSize < 8U)
    {
        error = 1;
    }
    else
    {
        uint64 tmp64;
        (void) memcpy((void*)&tmp64, (const void*)block->In, sizeof(uint64));
        data[0] = (uint8)(tmp64 & 0xFFU);
        data[1] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[4] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[5] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[6] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[7] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 8;
    }
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** No Save function                                                                                                 **/
/**********************************************************************************************************************/

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SAVESIGNAL_FLOAT64_ISLINKED)
void* SaveSignal_Float64_GetAddress(SAVESIGNAL_FLOAT64* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
