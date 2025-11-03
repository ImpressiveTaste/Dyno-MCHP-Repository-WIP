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
/* Description: One dimensional look-up table with selectable number of entries */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "LookupTable1D_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP32_Update, "ramfuncs")
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
void LookupTable1D_FiP32_Update(LOOKUPTABLE1D_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 tmp;
    uint16 idxx;
    int64 fact, factx, deltax;
    int64 inx;

    /** - check input boundaries */
    if (IN_X < MIN_X)
    {
        /* input below lower boundary -> cutoff */
        inx = -INT32_MAX;
    }
    else if (IN_X > MAX_X)
    {
        /* input above higher boundary -> cutoff */
        inx = INT32_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int64)IN_X - (int64)OFFSET_X);
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        inx = ((int64)tmp * (int64)GAIN_X) >> GAIN_X_SFR;
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
    tmp = ((int64)(factx - fact) * (int64)deltax) >> SFR_X; /* dx*(f(x+1) - f(x)) */
    tmp = fact + tmp;                                       /* f(x) + dx*(f(x+1) - f(x)) */

    OUT = (int32)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_FiP32_Init(LOOKUPTABLE1D_FIP32 *block)
{
    block->ID = LOOKUPTABLE1D_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_Load(const LOOKUPTABLE1D_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 24U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->sfrX & 0xFFU);
        data[1] = (uint8)(block->maskX & 0xFFU);
        data[2] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[3] = (uint8)((block->maskX >> 16) & 0xFFU);
        data[4] = (uint8)((block->maskX >> 24) & 0xFFU);
        data[5] = (uint8)(block->idxOffsetX & 0xFFU);
        data[6] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[7] = (uint8)((uint32)block->gainX & 0xFFU);
        data[8] = (uint8)(((uint32)block->gainX >> 8) & 0xFFU);
        data[9] = (uint8)(((uint32)block->gainX >> 16) & 0xFFU);
        data[10] = (uint8)(((uint32)block->gainX >> 24) & 0xFFU);
        data[11] = (uint8)(block->gainXsfr & 0xFFU);
        data[12] = (uint8)((uint32)block->offsetX & 0xFFU);
        data[13] = (uint8)(((uint32)block->offsetX >> 8) & 0xFFU);
        data[14] = (uint8)(((uint32)block->offsetX >> 16) & 0xFFU);
        data[15] = (uint8)(((uint32)block->offsetX >> 24) & 0xFFU);
        data[16] = (uint8)((uint32)block->minX & 0xFFU);
        data[17] = (uint8)(((uint32)block->minX >> 8) & 0xFFU);
        data[18] = (uint8)(((uint32)block->minX >> 16) & 0xFFU);
        data[19] = (uint8)(((uint32)block->minX >> 24) & 0xFFU);
        data[20] = (uint8)((uint32)block->maxX & 0xFFU);
        data[21] = (uint8)(((uint32)block->maxX >> 8) & 0xFFU);
        data[22] = (uint8)(((uint32)block->maxX >> 16) & 0xFFU);
        data[23] = (uint8)(((uint32)block->maxX >> 24) & 0xFFU);
        *dataLength = 24U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_Save(LOOKUPTABLE1D_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 24U)
    {
        error = 1U;
    }
    else
    {
        block->sfrX = data[0];
        block->maskX = (uint32)((uint32)data[1] | ((uint32)data[2] << 8) | ((uint32)data[3] << 16) | ((uint32)data[4] << 24));
        block->idxOffsetX = (uint16)((uint16)data[5] | ((uint16)data[6] << 8));
        block->gainX = castUint32ToInt32((uint32)((uint32)data[7] | ((uint32)data[8] << 8) | ((uint32)data[9] << 16) | ((uint32)data[10] << 24)));
        block->gainXsfr = data[11];
        block->offsetX = castUint32ToInt32((uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24)));
        block->minX = castUint32ToInt32((uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24)));
        block->maxX = castUint32ToInt32((uint32)((uint32)data[20] | ((uint32)data[21] << 8) | ((uint32)data[22] << 16) | ((uint32)data[23] << 24)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP32_ISLINKED)
void* LookupTable1D_FiP32_GetAddress(LOOKUPTABLE1D_FIP32* block, uint16 elementId)
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
