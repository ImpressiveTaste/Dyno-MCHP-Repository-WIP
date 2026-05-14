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
#define IN_X        (*pTLookupTable1D_FiP32_RAM->x)

/* Outputs */
#define OUT         (pTLookupTable1D_FiP32_RAM->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable1D_FiP32_RAM->Table)

#define SFR_X        (pTLookupTable1D_FiP32_RAM->sfrX)
#define MASK_X       (pTLookupTable1D_FiP32_RAM->maskX)
#define IDX_OFFSET_X (pTLookupTable1D_FiP32_RAM->idxOffsetX)
#define MIN_X        (pTLookupTable1D_FiP32_RAM->minX)
#define MAX_X        (pTLookupTable1D_FiP32_RAM->maxX)
#define GAIN_X       (pTLookupTable1D_FiP32_RAM->gainX)
#define GAIN_X_SFR   (pTLookupTable1D_FiP32_RAM->gainXsfr)
#define OFFSET_X     (pTLookupTable1D_FiP32_RAM->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_FiP32_RAM_Update(LOOKUPTABLE1D_FIP32_RAM *pTLookupTable1D_FiP32_RAM)
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
void LookupTable1D_FiP32_RAM_Init(LOOKUPTABLE1D_FIP32_RAM *pTLookupTable1D_FiP32_RAM)
{
    pTLookupTable1D_FiP32_RAM->ID = LOOKUPTABLE1D_FIP32_RAM_ID;
    pTLookupTable1D_FiP32_RAM->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_RAM_Load(const LOOKUPTABLE1D_FIP32_RAM *pTLookupTable1D_FiP32_RAM, uint8 data[], uint16 *dataLength, uint16 maxSize)
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
            data[(4U*i)] = (uint8)((uint32)pTLookupTable1D_FiP32_RAM->Table[i] & 0xFFU);
            data[(4U*i) + 1U] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->Table[i] >> 8) & 0xFFU);
            data[(4U*i) + 2U] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->Table[i] >> 16) & 0xFFU);
            data[(4U*i) + 3U] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->Table[i] >> 24) & 0xFFU);
        }
        data[1028] = (uint8)((uint8)pTLookupTable1D_FiP32_RAM->sfrX & 0xFFU);
        data[1029] = (uint8)(pTLookupTable1D_FiP32_RAM->maskX & 0xFFU);
        data[1030] = (uint8)((pTLookupTable1D_FiP32_RAM->maskX >> 8) & 0xFFU);
        data[1031] = (uint8)((pTLookupTable1D_FiP32_RAM->maskX >> 16) & 0xFFU);
        data[1032] = (uint8)((pTLookupTable1D_FiP32_RAM->maskX >> 24) & 0xFFU);
        data[1033] = (uint8)(pTLookupTable1D_FiP32_RAM->idxOffsetX & 0xFFU);
        data[1034] = (uint8)((pTLookupTable1D_FiP32_RAM->idxOffsetX >> 8) & 0xFFU);
        data[1035] = (uint8)((uint32)pTLookupTable1D_FiP32_RAM->gainX & 0xFFU);
        data[1036] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->gainX >> 8) & 0xFFU);
        data[1037] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->gainX >> 16) & 0xFFU);
        data[1038] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->gainX >> 24) & 0xFFU);
        data[1039] = (uint8)((uint8)pTLookupTable1D_FiP32_RAM->gainXsfr & 0xFFU);
        data[1040] = (uint8)((uint32)pTLookupTable1D_FiP32_RAM->offsetX & 0xFFU);
        data[1041] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->offsetX >> 8) & 0xFFU);
        data[1042] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->offsetX >> 16) & 0xFFU);
        data[1043] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->offsetX >> 24) & 0xFFU);
        data[1044] = (uint8)((uint32)pTLookupTable1D_FiP32_RAM->minX & 0xFFU);
        data[1045] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->minX >> 8) & 0xFFU);
        data[1046] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->minX >> 16) & 0xFFU);
        data[1047] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->minX >> 24) & 0xFFU);
        data[1048] = (uint8)((uint32)pTLookupTable1D_FiP32_RAM->maxX & 0xFFU);
        data[1049] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->maxX >> 8) & 0xFFU);
        data[1050] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->maxX >> 16) & 0xFFU);
        data[1051] = (uint8)(((uint32)pTLookupTable1D_FiP32_RAM->maxX >> 24) & 0xFFU);
        *dataLength = 1052U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP32_RAM_Save(LOOKUPTABLE1D_FIP32_RAM *pTLookupTable1D_FiP32_RAM, const uint8 data[], uint16 dataLength)
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
            pTLookupTable1D_FiP32_RAM->Table[i] = UINT32_TO_INT32((uint32)((uint32)data[(4U*i)] | ((uint32)data[(4U*i) + 1U] << 8) | ((uint32)data[(4U*i) + 2U] << 16) | ((uint32)data[(4U*i) + 3U] << 24)));
        }
        pTLookupTable1D_FiP32_RAM->sfrX = UINT8_TO_INT8(data[1028]);
        pTLookupTable1D_FiP32_RAM->maskX = (uint32)((uint32)data[1029] | ((uint32)data[1030] << 8) | ((uint32)data[1031] << 16) | ((uint32)data[1032] << 24));
        pTLookupTable1D_FiP32_RAM->idxOffsetX = (uint16)((uint16)data[1033] | ((uint16)data[1034] << 8));
        pTLookupTable1D_FiP32_RAM->gainX = UINT32_TO_INT32((uint32)((uint32)data[1035] | ((uint32)data[1036] << 8) | ((uint32)data[1037] << 16) | ((uint32)data[1038] << 24)));
        pTLookupTable1D_FiP32_RAM->gainXsfr = UINT8_TO_INT8(data[1039]);
        pTLookupTable1D_FiP32_RAM->offsetX = UINT32_TO_INT32((uint32)((uint32)data[1040] | ((uint32)data[1041] << 8) | ((uint32)data[1042] << 16) | ((uint32)data[1043] << 24)));
        pTLookupTable1D_FiP32_RAM->minX = UINT32_TO_INT32((uint32)((uint32)data[1044] | ((uint32)data[1045] << 8) | ((uint32)data[1046] << 16) | ((uint32)data[1047] << 24)));
        pTLookupTable1D_FiP32_RAM->maxX = UINT32_TO_INT32((uint32)((uint32)data[1048] | ((uint32)data[1049] << 8) | ((uint32)data[1050] << 16) | ((uint32)data[1051] << 24)));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP32_RAM_ISLINKED)
void* LookupTable1D_FiP32_RAM_GetAddress(const LOOKUPTABLE1D_FIP32_RAM* block, uint16 elementId)
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
