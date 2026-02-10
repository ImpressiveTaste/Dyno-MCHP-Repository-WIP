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
/**     Description:    Saturation of output to adjustable upper and lower   **/
/**                     limit.                                               **/
/**                     Calculation:                                         **/
/**                         Out = In                                         **/
/**                             --> with Out = [min..max]                    **/
/**                     Range:                                               **/
/**                         In:     -1..1                                    **/
/**                         Out:    min..max                                 **/
/**                         min:    -1..max                                  **/
/**                         max:    min..1                                   **/
/**                                                                          **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Saturation_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Saturation_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN      (*block->In)       /* Q15 */

/* Outputs */
#define OUT     (block->Out)       /* Q15 */

/* Parameter */
#define MAX     (block->max)       /* upper limit, Q15 */
#define MIN     (block->min)       /* lower limit, Q15 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Saturation_FiP16_Update(SATURATION_FIP16 *block)
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
void Saturation_FiP16_Init(SATURATION_FIP16 *block)
{
    block->ID = SATURATION_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Saturation_FiP16_Load(const SATURATION_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 4U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->max & 0xFFU);
        data[1] = (uint8)(((uint16)block->max >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->min & 0xFFU);
        data[3] = (uint8)(((uint16)block->min >> 8) & 0xFFU);
        *dataLength = 4U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Saturation_FiP16_Save(SATURATION_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 4U)
    {
        error = 1U;
    }
    else
    {
        block->max = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->min = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SATURATION_FIP16_ISLINKED)
void* Saturation_FiP16_GetAddress(SATURATION_FIP16* block, uint16 elementId)
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
