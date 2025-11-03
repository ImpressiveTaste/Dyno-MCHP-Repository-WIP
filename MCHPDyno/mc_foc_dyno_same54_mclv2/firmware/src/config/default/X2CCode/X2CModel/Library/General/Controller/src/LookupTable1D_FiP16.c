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
#include "LookupTable1D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP16_Update, "ramfuncs")
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
void LookupTable1D_FiP16_Update(LOOKUPTABLE1D_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
#ifdef true
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
#else
    int32 tmp;
    uint16 idxx;
    int16 fact, factx, deltax;
    int16 inx;

    /* boundary check */
    if (IN_X < MIN_X)
    {
        /* input below lower boundary -> cutoff */
        tmp = TABLE_DATA[0];        /* f(0) */
    }
    else if (IN_X > MAX_X)
    {
        /* input above higher boundary -> cutoff */
        tmp = TABLE_DATA[DIM_X];    /* f(end) */
    }
    else
    {
        /* input within boundaries -> interpolation */
        /* input scaling */
        tmp = ((int32)IN_X - (int32)OFFSET_X);
        inx = ((int32)tmp * (int32)GAIN_X) >> GAIN_X_SFR;

        /* calculation of index */
        idxx = (inx >> SFR_X) + IDX_OFFSET_X;                   /*  x */

        /* calculation of values at lattice points */
        fact  = TABLE_DATA[idxx];                               /* f(x) */
        factx = TABLE_DATA[idxx + 1];                           /* f(x+1) */

        /* interpolation */
        deltax = inx & MASK_X;                                  /* dx */
        tmp = ((int32)(factx - fact) * (int32)deltax) >> SFR_X; /* dx*(f(x+1) - f(x)) */
        tmp = fact + tmp;                                       /* f(x) + dx*(f(x+1) - f(x)) */
    }

    OUT = (int16)tmp;
#endif
/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_Init(LOOKUPTABLE1D_FIP16 *block)
{
    block->ID = LOOKUPTABLE1D_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_Load(const LOOKUPTABLE1D_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 14U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->sfrX & 0xFFU);
        data[1] = (uint8)(block->maskX & 0xFFU);
        data[2] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[3] = (uint8)(block->idxOffsetX & 0xFFU);
        data[4] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[5] = (uint8)((uint16)block->gainX & 0xFFU);
        data[6] = (uint8)(((uint16)block->gainX >> 8) & 0xFFU);
        data[7] = (uint8)(block->gainXsfr & 0xFFU);
        data[8] = (uint8)((uint16)block->offsetX & 0xFFU);
        data[9] = (uint8)(((uint16)block->offsetX >> 8) & 0xFFU);
        data[10] = (uint8)((uint16)block->minX & 0xFFU);
        data[11] = (uint8)(((uint16)block->minX >> 8) & 0xFFU);
        data[12] = (uint8)((uint16)block->maxX & 0xFFU);
        data[13] = (uint8)(((uint16)block->maxX >> 8) & 0xFFU);
        *dataLength = 14U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_Save(LOOKUPTABLE1D_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 14U)
    {
        error = 1U;
    }
    else
    {
        block->sfrX = data[0];
        block->maskX = (uint16)((uint16)data[1] | ((uint16)data[2] << 8));
        block->idxOffsetX = (uint16)((uint16)data[3] | ((uint16)data[4] << 8));
        block->gainX = castUint16ToInt16((uint16)((uint16)data[5] | ((uint16)data[6] << 8)));
        block->gainXsfr = data[7];
        block->offsetX = castUint16ToInt16((uint16)((uint16)data[8] | ((uint16)data[9] << 8)));
        block->minX = castUint16ToInt16((uint16)((uint16)data[10] | ((uint16)data[11] << 8)));
        block->maxX = castUint16ToInt16((uint16)((uint16)data[12] | ((uint16)data[13] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP16_ISLINKED)
void* LookupTable1D_FiP16_GetAddress(LOOKUPTABLE1D_FIP16* block, uint16 elementId)
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
