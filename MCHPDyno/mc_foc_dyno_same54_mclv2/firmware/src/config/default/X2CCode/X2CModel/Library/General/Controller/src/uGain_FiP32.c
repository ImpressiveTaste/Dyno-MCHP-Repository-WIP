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
/*     Description: Amplification of Input by Gain factor with output wrapping
 *                      Calculation:
 *                          Out = In * V
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "uGain_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(uGain_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)            /* Q31 */

/* Outputs */
#define OUT             (block->Out)            /* Q31 */

/* Parameter */
#define GAIN            (block->V)              /* Qx */
#define SFR             (block->sfr)            /* x */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void uGain_FiP32_Update(UGAIN_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 result;

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001 ", true_no_defect) */
    result = ((int64)IN * (int64)GAIN) >> SFR;

    OUT = (int32)result;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void uGain_FiP32_Init(UGAIN_FIP32 *block)
{
    block->ID = UGAIN_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 uGain_FiP32_Load(const UGAIN_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 5U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->V & 0xFFU);
        data[1] = (uint8)(((uint32)block->V >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->V >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->V >> 24) & 0xFFU);
        data[4] = (uint8)(block->sfr & 0xFFU);
        *dataLength = 5U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 uGain_FiP32_Save(UGAIN_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 5U)
    {
        error = 1U;
    }
    else
    {
        block->V = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->sfr = data[4];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(UGAIN_FIP32_ISLINKED)
void* uGain_FiP32_GetAddress(UGAIN_FIP32* block, uint16 elementId)
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
