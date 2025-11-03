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
#include "LookupTable1D_FiP16_RAM.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP16_RAM_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*block->x)

/* Outputs */
#define OUT         (block->Out)

/* Parameters */
#define TABLE_DATA   (block->Table)

#define SFR_X        (block->sfrX)
#define MASK_X       (block->maskX)
#define IDX_OFFSET_X (block->idxOffsetX)
#define MIN_X        (block->minX)
#define MAX_X        (block->maxX)
#define GAIN_X       (block->gainX)
#define GAIN_X_SFR   (block->gainXsfr)
#define OFFSET_X     (block->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_RAM_Update(LOOKUPTABLE1D_FIP16_RAM *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 tmp;
    uint16 idxx;
    int16 fact, factx, deltax;
    int16 inx;

    /** - check input boundaries */
    if (IN_X < MIN_X)
    {
        /* input below lower boundary -> cutoff */
        inx = -INT16_MAX;
    }
    else if (IN_X > MAX_X)
    {
        /* input above higher boundary -> cutoff */
        inx = INT16_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int32)IN_X - (int32)OFFSET_X);
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        inx = ((int32)tmp * (int32)GAIN_X) >> GAIN_X_SFR;
    }

    /** - calculate table index */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    idxx = (inx >> SFR_X) + IDX_OFFSET_X;                   /*  x */

    /** - calculate fractual part */
    deltax = inx & MASK_X;                                  /* dx */

    /** - calculate value at lattice points */
    fact  = TABLE_DATA[idxx];                               /* f(x) */
    factx = TABLE_DATA[idxx + 1];                           /* f(x+1) */

    /** - linear interpolation */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    tmp = ((int32)(factx - fact) * (int32)deltax) >> SFR_X; /* dx*(f(x+1) - f(x)) */
    tmp = fact + tmp;                                       /* f(x) + dx*(f(x+1) - f(x)) */

    OUT = (int16)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_RAM_Init(LOOKUPTABLE1D_FIP16_RAM *block)
{
    block->ID = LOOKUPTABLE1D_FIP16_RAM_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_RAM_Load(const LOOKUPTABLE1D_FIP16_RAM *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 528U)
    {
        error = 1U;
    }
    else
    {
        uint16 i;
        for (i=0; i<257U; i++)
        {
            data[(2U*i)] = (uint8)((uint16)block->Table[i] & 0xFFU);
            data[(2U*i) + 1U] = (uint8)(((uint16)block->Table[i] >> 8) & 0xFFU);
        }
        data[514] = (uint8)(block->sfrX & 0xFFU);
        data[515] = (uint8)(block->maskX & 0xFFU);
        data[516] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[517] = (uint8)(block->idxOffsetX & 0xFFU);
        data[518] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[519] = (uint8)((uint16)block->gainX & 0xFFU);
        data[520] = (uint8)(((uint16)block->gainX >> 8) & 0xFFU);
        data[521] = (uint8)(block->gainXsfr & 0xFFU);
        data[522] = (uint8)((uint16)block->offsetX & 0xFFU);
        data[523] = (uint8)(((uint16)block->offsetX >> 8) & 0xFFU);
        data[524] = (uint8)((uint16)block->minX & 0xFFU);
        data[525] = (uint8)(((uint16)block->minX >> 8) & 0xFFU);
        data[526] = (uint8)((uint16)block->maxX & 0xFFU);
        data[527] = (uint8)(((uint16)block->maxX >> 8) & 0xFFU);
        *dataLength = 528U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_RAM_Save(LOOKUPTABLE1D_FIP16_RAM *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 528U)
    {
        error = 1U;
    }
    else
    {
        uint16 i;
        for (i=0; i<257U; i++)
        {
            block->Table[i] = castUint16ToInt16((uint16)((uint16)data[(2U*i)] | ((uint16)data[(2U*i) + 1U] << 8)));
        }
        block->sfrX = data[514];
        block->maskX = (uint16)((uint16)data[515] | ((uint16)data[516] << 8));
        block->idxOffsetX = (uint16)((uint16)data[517] | ((uint16)data[518] << 8));
        block->gainX = castUint16ToInt16((uint16)((uint16)data[519] | ((uint16)data[520] << 8)));
        block->gainXsfr = data[521];
        block->offsetX = castUint16ToInt16((uint16)((uint16)data[522] | ((uint16)data[523] << 8)));
        block->minX = castUint16ToInt16((uint16)((uint16)data[524] | ((uint16)data[525] << 8)));
        block->maxX = castUint16ToInt16((uint16)((uint16)data[526] | ((uint16)data[527] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP16_RAM_ISLINKED)
void* LookupTable1D_FiP16_RAM_GetAddress(LOOKUPTABLE1D_FIP16_RAM* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->x;
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
