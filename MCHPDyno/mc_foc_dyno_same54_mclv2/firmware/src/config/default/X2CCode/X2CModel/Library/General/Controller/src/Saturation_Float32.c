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
/* Description: */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Saturation_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Saturation_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN      (*block->In)

/* Outputs */
#define OUT     (block->Out)

/* Parameter */
#define MAX     (block->max)     /* upper limit */
#define MIN     (block->min)     /* lower limit */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Saturation_Float32_Update(SATURATION_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    OUT = IN;
    if (OUT > MAX)
    {
        /* input signal is higher than maximum allowed value */
        OUT = MAX;
    }
    else if (OUT < MIN)
    {
        /* input signal is lower than minimum allowed value */
        OUT = MIN;
    }
    else
    {
        /* else: No operation required */
    }

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Saturation_Float32_Init(SATURATION_FLOAT32 *block)
{
    block->ID = SATURATION_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Saturation_Float32_Load(const SATURATION_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 8U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        (void) memcpy((void*)&tmp32, (const void*)&block->max, sizeof(uint32));
        data[0] = (uint8)(tmp32 & 0xFFU);
        data[1] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->min, sizeof(uint32));
        data[4] = (uint8)(tmp32 & 0xFFU);
        data[5] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[6] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[7] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 8U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Saturation_Float32_Save(SATURATION_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 8U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        tmp32 = (uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24));
        (void) memcpy((void*)&block->max, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24));
        (void) memcpy((void*)&block->min, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SATURATION_FLOAT32_ISLINKED)
void* Saturation_Float32_GetAddress(SATURATION_FLOAT32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
