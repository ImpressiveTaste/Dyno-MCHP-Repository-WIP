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
#include "LookupTable1D_FiP32_RAM.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP32_RAM_Update, "ramfuncs")
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
void LookupTable1D_FiP32_RAM_Update(LOOKUPTABLE1D_FIP32_RAM *block)
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
void LookupTable1D_FiP32_RAM_Init(LOOKUPTABLE1D_FIP32_RAM *block)
{
    block->ID = LOOKUPTABLE1D_FIP32_RAM_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_RAM_Load(const LOOKUPTABLE1D_FIP32_RAM *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 1052U)
    {
        error = 1U;
    }
    else
    {
        uint16 i;
        for (i=0; i<257U; i++)
        {
            data[(4U*i)] = (uint8)((uint32)block->Table[i] & 0xFFU);
            data[(4U*i) + 1U] = (uint8)(((uint32)block->Table[i] >> 8) & 0xFFU);
            data[(4U*i) + 2U] = (uint8)(((uint32)block->Table[i] >> 16) & 0xFFU);
            data[(4U*i) + 3U] = (uint8)(((uint32)block->Table[i] >> 24) & 0xFFU);
        }
        data[1028] = (uint8)(block->sfrX & 0xFFU);
        data[1029] = (uint8)(block->maskX & 0xFFU);
        data[1030] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[1031] = (uint8)((block->maskX >> 16) & 0xFFU);
        data[1032] = (uint8)((block->maskX >> 24) & 0xFFU);
        data[1033] = (uint8)(block->idxOffsetX & 0xFFU);
        data[1034] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[1035] = (uint8)((uint32)block->gainX & 0xFFU);
        data[1036] = (uint8)(((uint32)block->gainX >> 8) & 0xFFU);
        data[1037] = (uint8)(((uint32)block->gainX >> 16) & 0xFFU);
        data[1038] = (uint8)(((uint32)block->gainX >> 24) & 0xFFU);
        data[1039] = (uint8)(block->gainXsfr & 0xFFU);
        data[1040] = (uint8)((uint32)block->offsetX & 0xFFU);
        data[1041] = (uint8)(((uint32)block->offsetX >> 8) & 0xFFU);
        data[1042] = (uint8)(((uint32)block->offsetX >> 16) & 0xFFU);
        data[1043] = (uint8)(((uint32)block->offsetX >> 24) & 0xFFU);
        data[1044] = (uint8)((uint32)block->minX & 0xFFU);
        data[1045] = (uint8)(((uint32)block->minX >> 8) & 0xFFU);
        data[1046] = (uint8)(((uint32)block->minX >> 16) & 0xFFU);
        data[1047] = (uint8)(((uint32)block->minX >> 24) & 0xFFU);
        data[1048] = (uint8)((uint32)block->maxX & 0xFFU);
        data[1049] = (uint8)(((uint32)block->maxX >> 8) & 0xFFU);
        data[1050] = (uint8)(((uint32)block->maxX >> 16) & 0xFFU);
        data[1051] = (uint8)(((uint32)block->maxX >> 24) & 0xFFU);
        *dataLength = 1052U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_RAM_Save(LOOKUPTABLE1D_FIP32_RAM *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 1052U)
    {
        error = 1U;
    }
    else
    {
        uint16 i;
        for (i=0; i<257U; i++)
        {
            block->Table[i] = castUint32ToInt32((uint32)((uint32)data[(4U*i)] | ((uint32)data[(4U*i) + 1U] << 8) | ((uint32)data[(4U*i) + 2U] << 16) | ((uint32)data[(4U*i) + 3U] << 24)));
        }
        block->sfrX = data[1028];
        block->maskX = (uint32)((uint32)data[1029] | ((uint32)data[1030] << 8) | ((uint32)data[1031] << 16) | ((uint32)data[1032] << 24));
        block->idxOffsetX = (uint16)((uint16)data[1033] | ((uint16)data[1034] << 8));
        block->gainX = castUint32ToInt32((uint32)((uint32)data[1035] | ((uint32)data[1036] << 8) | ((uint32)data[1037] << 16) | ((uint32)data[1038] << 24)));
        block->gainXsfr = data[1039];
        block->offsetX = castUint32ToInt32((uint32)((uint32)data[1040] | ((uint32)data[1041] << 8) | ((uint32)data[1042] << 16) | ((uint32)data[1043] << 24)));
        block->minX = castUint32ToInt32((uint32)((uint32)data[1044] | ((uint32)data[1045] << 8) | ((uint32)data[1046] << 16) | ((uint32)data[1047] << 24)));
        block->maxX = castUint32ToInt32((uint32)((uint32)data[1048] | ((uint32)data[1049] << 8) | ((uint32)data[1050] << 16) | ((uint32)data[1051] << 24)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP32_RAM_ISLINKED)
void* LookupTable1D_FiP32_RAM_GetAddress(LOOKUPTABLE1D_FIP32_RAM* block, uint16 elementId)
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
