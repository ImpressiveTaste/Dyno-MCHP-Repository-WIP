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
#include "LookupTable1D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*pTLookupTable1D_FiP16->x)

/* Outputs */
#define OUT         (pTLookupTable1D_FiP16->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable1D_FiP16->Table)

#define SFR_X        (pTLookupTable1D_FiP16->sfrX)
#define MASK_X       (pTLookupTable1D_FiP16->maskX)
#define IDX_OFFSET_X (pTLookupTable1D_FiP16->idxOffsetX)
#define MIN_X        (pTLookupTable1D_FiP16->minX)
#define MAX_X        (pTLookupTable1D_FiP16->maxX)
#define GAIN_X       (pTLookupTable1D_FiP16->gainX)
#define GAIN_X_SFR   (pTLookupTable1D_FiP16->gainXsfr)
#define OFFSET_X     (pTLookupTable1D_FiP16->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_FiP16_Update(LOOKUPTABLE1D_FIP16 *pTLookupTable1D_FiP16)
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
void LookupTable1D_FiP16_Init(LOOKUPTABLE1D_FIP16 *pTLookupTable1D_FiP16)
{
    pTLookupTable1D_FiP16->ID = LOOKUPTABLE1D_FIP16_ID;
    pTLookupTable1D_FiP16->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_Load(const LOOKUPTABLE1D_FIP16 *pTLookupTable1D_FiP16, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 14U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint8)pTLookupTable1D_FiP16->sfrX & 0xFFU);
        data[1] = (uint8)(pTLookupTable1D_FiP16->maskX & 0xFFU);
        data[2] = (uint8)((pTLookupTable1D_FiP16->maskX >> 8) & 0xFFU);
        data[3] = (uint8)(pTLookupTable1D_FiP16->idxOffsetX & 0xFFU);
        data[4] = (uint8)((pTLookupTable1D_FiP16->idxOffsetX >> 8) & 0xFFU);
        data[5] = (uint8)((uint16)pTLookupTable1D_FiP16->gainX & 0xFFU);
        data[6] = (uint8)(((uint16)pTLookupTable1D_FiP16->gainX >> 8) & 0xFFU);
        data[7] = (uint8)((uint8)pTLookupTable1D_FiP16->gainXsfr & 0xFFU);
        data[8] = (uint8)((uint16)pTLookupTable1D_FiP16->offsetX & 0xFFU);
        data[9] = (uint8)(((uint16)pTLookupTable1D_FiP16->offsetX >> 8) & 0xFFU);
        data[10] = (uint8)((uint16)pTLookupTable1D_FiP16->minX & 0xFFU);
        data[11] = (uint8)(((uint16)pTLookupTable1D_FiP16->minX >> 8) & 0xFFU);
        data[12] = (uint8)((uint16)pTLookupTable1D_FiP16->maxX & 0xFFU);
        data[13] = (uint8)(((uint16)pTLookupTable1D_FiP16->maxX >> 8) & 0xFFU);
        *dataLength = 14U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_FiP16_Save(LOOKUPTABLE1D_FIP16 *pTLookupTable1D_FiP16, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != 14U)
    {
        error = 1U;
    }
    else
    {
        pTLookupTable1D_FiP16->sfrX = UINT8_TO_INT8(data[0]);
        pTLookupTable1D_FiP16->maskX = (uint16)((uint16)data[1] | ((uint16)data[2] << 8));
        pTLookupTable1D_FiP16->idxOffsetX = (uint16)((uint16)data[3] | ((uint16)data[4] << 8));
        pTLookupTable1D_FiP16->gainX = UINT16_TO_INT16((uint16)((uint16)data[5] | ((uint16)data[6] << 8)));
        pTLookupTable1D_FiP16->gainXsfr = UINT8_TO_INT8(data[7]);
        pTLookupTable1D_FiP16->offsetX = UINT16_TO_INT16((uint16)((uint16)data[8] | ((uint16)data[9] << 8)));
        pTLookupTable1D_FiP16->minX = UINT16_TO_INT16((uint16)((uint16)data[10] | ((uint16)data[11] << 8)));
        pTLookupTable1D_FiP16->maxX = UINT16_TO_INT16((uint16)((uint16)data[12] | ((uint16)data[13] << 8)));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FIP16_ISLINKED)
void* LookupTable1D_FiP16_GetAddress(const LOOKUPTABLE1D_FIP16* block, uint16 elementId)
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
