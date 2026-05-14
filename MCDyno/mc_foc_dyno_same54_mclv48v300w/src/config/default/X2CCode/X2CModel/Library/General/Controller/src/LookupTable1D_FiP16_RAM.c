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
#include "LookupTable1D_FiP16_RAM.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP16_RAM_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*pTLookupTable1D_FiP16_RAM->x)

/* Outputs */
#define OUT         (pTLookupTable1D_FiP16_RAM->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable1D_FiP16_RAM->Table)

#define SFR_X        (pTLookupTable1D_FiP16_RAM->sfrX)
#define MASK_X       (pTLookupTable1D_FiP16_RAM->maskX)
#define IDX_OFFSET_X (pTLookupTable1D_FiP16_RAM->idxOffsetX)
#define MIN_X        (pTLookupTable1D_FiP16_RAM->minX)
#define MAX_X        (pTLookupTable1D_FiP16_RAM->maxX)
#define GAIN_X       (pTLookupTable1D_FiP16_RAM->gainX)
#define GAIN_X_SFR   (pTLookupTable1D_FiP16_RAM->gainXsfr)
#define OFFSET_X     (pTLookupTable1D_FiP16_RAM->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_RAM_Update(LOOKUPTABLE1D_FIP16_RAM *pTLookupTable1D_FiP16_RAM)
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
        inx = ((int32)tmp * (int32)GAIN_X) >> GAIN_X_SFR;
    }

    /** - calculate table index */
    idxx = (inx >> SFR_X) + IDX_OFFSET_X;                   /*  x */

    /** - calculate fractual part */
    deltax = inx & MASK_X;                                  /* dx */

    /** - calculate value at lattice points */
    fact  = TABLE_DATA[idxx];                               /* f(x) */
    factx = TABLE_DATA[idxx + 1];                           /* f(x+1) */

    /** - linear interpolation */
    tmp = ((int32)(factx - fact) * (int32)deltax) >> SFR_X; /* dx*(f(x+1) - f(x)) */
    tmp = fact + tmp;                                       /* f(x) + dx*(f(x+1) - f(x)) */

    OUT = (int16)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_RAM_Init(LOOKUPTABLE1D_FIP16_RAM *pTLookupTable1D_FiP16_RAM)
{
    pTLookupTable1D_FiP16_RAM->ID = LOOKUPTABLE1D_FIP16_RAM_ID;
    pTLookupTable1D_FiP16_RAM->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_RAM_Load(const LOOKUPTABLE1D_FIP16_RAM *pTLookupTable1D_FiP16_RAM, uint8 data[], uint16 *dataLength, uint16 maxSize)
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
            data[(2U*i)] = (uint8)((uint16)pTLookupTable1D_FiP16_RAM->Table[i] & 0xFFU);
            data[(2U*i) + 1U] = (uint8)(((uint16)pTLookupTable1D_FiP16_RAM->Table[i] >> 8) & 0xFFU);
        }
        data[514] = (uint8)((uint8)pTLookupTable1D_FiP16_RAM->sfrX & 0xFFU);
        data[515] = (uint8)(pTLookupTable1D_FiP16_RAM->maskX & 0xFFU);
        data[516] = (uint8)((pTLookupTable1D_FiP16_RAM->maskX >> 8) & 0xFFU);
        data[517] = (uint8)(pTLookupTable1D_FiP16_RAM->idxOffsetX & 0xFFU);
        data[518] = (uint8)((pTLookupTable1D_FiP16_RAM->idxOffsetX >> 8) & 0xFFU);
        data[519] = (uint8)((uint16)pTLookupTable1D_FiP16_RAM->gainX & 0xFFU);
        data[520] = (uint8)(((uint16)pTLookupTable1D_FiP16_RAM->gainX >> 8) & 0xFFU);
        data[521] = (uint8)((uint8)pTLookupTable1D_FiP16_RAM->gainXsfr & 0xFFU);
        data[522] = (uint8)((uint16)pTLookupTable1D_FiP16_RAM->offsetX & 0xFFU);
        data[523] = (uint8)(((uint16)pTLookupTable1D_FiP16_RAM->offsetX >> 8) & 0xFFU);
        data[524] = (uint8)((uint16)pTLookupTable1D_FiP16_RAM->minX & 0xFFU);
        data[525] = (uint8)(((uint16)pTLookupTable1D_FiP16_RAM->minX >> 8) & 0xFFU);
        data[526] = (uint8)((uint16)pTLookupTable1D_FiP16_RAM->maxX & 0xFFU);
        data[527] = (uint8)(((uint16)pTLookupTable1D_FiP16_RAM->maxX >> 8) & 0xFFU);
        *dataLength = 528U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_RAM_Save(LOOKUPTABLE1D_FIP16_RAM *pTLookupTable1D_FiP16_RAM, const uint8 data[], uint16 dataLength)
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
            pTLookupTable1D_FiP16_RAM->Table[i] = UINT16_TO_INT16((uint16)((uint16)data[(2U*i)] | ((uint16)data[(2U*i) + 1U] << 8)));
        }
        pTLookupTable1D_FiP16_RAM->sfrX = UINT8_TO_INT8(data[514]);
        pTLookupTable1D_FiP16_RAM->maskX = (uint16)((uint16)data[515] | ((uint16)data[516] << 8));
        pTLookupTable1D_FiP16_RAM->idxOffsetX = (uint16)((uint16)data[517] | ((uint16)data[518] << 8));
        pTLookupTable1D_FiP16_RAM->gainX = UINT16_TO_INT16((uint16)((uint16)data[519] | ((uint16)data[520] << 8)));
        pTLookupTable1D_FiP16_RAM->gainXsfr = UINT8_TO_INT8(data[521]);
        pTLookupTable1D_FiP16_RAM->offsetX = UINT16_TO_INT16((uint16)((uint16)data[522] | ((uint16)data[523] << 8)));
        pTLookupTable1D_FiP16_RAM->minX = UINT16_TO_INT16((uint16)((uint16)data[524] | ((uint16)data[525] << 8)));
        pTLookupTable1D_FiP16_RAM->maxX = UINT16_TO_INT16((uint16)((uint16)data[526] | ((uint16)data[527] << 8)));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP16_RAM_ISLINKED)
void* LookupTable1D_FiP16_RAM_GetAddress(const LOOKUPTABLE1D_FIP16_RAM* block, uint16 elementId)
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
