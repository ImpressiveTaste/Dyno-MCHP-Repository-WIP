/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM) http://www.lcm.at/
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
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 3363 $
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
#define IN_X        (*pTLookupTable1D_FiP32->x)

/* Outputs */
#define OUT         (pTLookupTable1D_FiP32->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable1D_FiP32->Table)

#define SFR_X        (pTLookupTable1D_FiP32->sfrX)
#define MASK_X       (pTLookupTable1D_FiP32->maskX)
#define IDX_OFFSET_X (pTLookupTable1D_FiP32->idxOffsetX)
#define MIN_X        (pTLookupTable1D_FiP32->minX)
#define MAX_X        (pTLookupTable1D_FiP32->maxX)
#define GAIN_X       (pTLookupTable1D_FiP32->gainX)
#define GAIN_X_SFR   (pTLookupTable1D_FiP32->gainXsfr)
#define OFFSET_X     (pTLookupTable1D_FiP32->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_FiP32_Update(LOOKUPTABLE1D_FIP32 *pTLookupTable1D_FiP32)
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
        inx = ((int64)tmp * (int64)GAIN_X) >> GAIN_X_SFR;
    }
        
    /** - calculate table index */
    idxx = (inx >> SFR_X) + IDX_OFFSET_X;                   /*  x */
    
    /** - calculate fractual part */
    deltax = inx & MASK_X;                                  /* dx */
    
    /** - calculate value at lattice points */
    fact  = TABLE_DATA[idxx];                               /* f(x) */
    factx = TABLE_DATA[idxx + 1];                           /* f(x+1) */

    /** - linear interpolation */
    tmp = ((int64)(factx - fact) * (int64)deltax) >> SFR_X; /* dx*(f(x+1) - f(x)) */
    tmp = fact + tmp;                                       /* f(x) + dx*(f(x+1) - f(x)) */

    OUT = (int32)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_FiP32_Init(LOOKUPTABLE1D_FIP32 *pTLookupTable1D_FiP32)
{
    pTLookupTable1D_FiP32->ID = LOOKUPTABLE1D_FIP32_ID;
    pTLookupTable1D_FiP32->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_Load(const LOOKUPTABLE1D_FIP32 *pTLookupTable1D_FiP32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 24U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint8)pTLookupTable1D_FiP32->sfrX & 0xFFU);
        data[1] = (uint8)(pTLookupTable1D_FiP32->maskX & 0xFFU);
        data[2] = (uint8)((pTLookupTable1D_FiP32->maskX >> 8) & 0xFFU);
        data[3] = (uint8)((pTLookupTable1D_FiP32->maskX >> 16) & 0xFFU);
        data[4] = (uint8)((pTLookupTable1D_FiP32->maskX >> 24) & 0xFFU);
        data[5] = (uint8)(pTLookupTable1D_FiP32->idxOffsetX & 0xFFU);
        data[6] = (uint8)((pTLookupTable1D_FiP32->idxOffsetX >> 8) & 0xFFU);
        data[7] = (uint8)((uint32)pTLookupTable1D_FiP32->gainX & 0xFFU);
        data[8] = (uint8)(((uint32)pTLookupTable1D_FiP32->gainX >> 8) & 0xFFU);
        data[9] = (uint8)(((uint32)pTLookupTable1D_FiP32->gainX >> 16) & 0xFFU);
        data[10] = (uint8)(((uint32)pTLookupTable1D_FiP32->gainX >> 24) & 0xFFU);
        data[11] = (uint8)((uint8)pTLookupTable1D_FiP32->gainXsfr & 0xFFU);
        data[12] = (uint8)((uint32)pTLookupTable1D_FiP32->offsetX & 0xFFU);
        data[13] = (uint8)(((uint32)pTLookupTable1D_FiP32->offsetX >> 8) & 0xFFU);
        data[14] = (uint8)(((uint32)pTLookupTable1D_FiP32->offsetX >> 16) & 0xFFU);
        data[15] = (uint8)(((uint32)pTLookupTable1D_FiP32->offsetX >> 24) & 0xFFU);
        data[16] = (uint8)((uint32)pTLookupTable1D_FiP32->minX & 0xFFU);
        data[17] = (uint8)(((uint32)pTLookupTable1D_FiP32->minX >> 8) & 0xFFU);
        data[18] = (uint8)(((uint32)pTLookupTable1D_FiP32->minX >> 16) & 0xFFU);
        data[19] = (uint8)(((uint32)pTLookupTable1D_FiP32->minX >> 24) & 0xFFU);
        data[20] = (uint8)((uint32)pTLookupTable1D_FiP32->maxX & 0xFFU);
        data[21] = (uint8)(((uint32)pTLookupTable1D_FiP32->maxX >> 8) & 0xFFU);
        data[22] = (uint8)(((uint32)pTLookupTable1D_FiP32->maxX >> 16) & 0xFFU);
        data[23] = (uint8)(((uint32)pTLookupTable1D_FiP32->maxX >> 24) & 0xFFU);
        *dataLength = 24U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_Save(LOOKUPTABLE1D_FIP32 *pTLookupTable1D_FiP32, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != 24U)
    {
        error = 1U;
    }
    else
    {
        pTLookupTable1D_FiP32->sfrX = UINT8_TO_INT8(data[0]);
        pTLookupTable1D_FiP32->maskX = (uint32)((uint32)data[1] | ((uint32)data[2] << 8) | ((uint32)data[3] << 16) | ((uint32)data[4] << 24));
        pTLookupTable1D_FiP32->idxOffsetX = (uint16)((uint16)data[5] | ((uint16)data[6] << 8));
        pTLookupTable1D_FiP32->gainX = UINT32_TO_INT32((uint32)((uint32)data[7] | ((uint32)data[8] << 8) | ((uint32)data[9] << 16) | ((uint32)data[10] << 24)));
        pTLookupTable1D_FiP32->gainXsfr = UINT8_TO_INT8(data[11]);
        pTLookupTable1D_FiP32->offsetX = UINT32_TO_INT32((uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24)));
        pTLookupTable1D_FiP32->minX = UINT32_TO_INT32((uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24)));
        pTLookupTable1D_FiP32->maxX = UINT32_TO_INT32((uint32)((uint32)data[20] | ((uint32)data[21] << 8) | ((uint32)data[22] << 16) | ((uint32)data[23] << 24)));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP32_ISLINKED)
void* LookupTable1D_FiP32_GetAddress(const LOOKUPTABLE1D_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = (void*)block->x;
            break;
        case 2:
            addr = (void*)&block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
