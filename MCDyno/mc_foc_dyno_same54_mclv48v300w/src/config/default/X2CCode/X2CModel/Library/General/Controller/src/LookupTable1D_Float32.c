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
#include "LookupTable1D_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable1D_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*pTLookupTable1D_Float32->x)

/* Outputs */
#define OUT         (pTLookupTable1D_Float32->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable1D_Float32->Table)

#define DIM_X        (pTLookupTable1D_Float32->dimX)
#define GAIN_X       (pTLookupTable1D_Float32->gainX)
#define OFFSET_X     (pTLookupTable1D_Float32->offsetX)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable1D_Float32_Update(LOOKUPTABLE1D_FLOAT32 *pTLookupTable1D_Float32)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 tmp;
    uint16 idxx;
    float32 fact, factx, deltax;
    float32 inx;
    
    /** - scale input */
    inx = (IN_X - OFFSET_X) * GAIN_X;

    /** - check boundaries */
    if (inx < 0)
    {
        /* input below lower boundary -> cutoff */
        tmp = TABLE_DATA[0];       /* f(0) */
    }
    else if ((uint16)inx >= DIM_X) /* uint16 cast is needed to get floor value of input */
    {
        /* input above higher boundary -> cutoff */
        tmp = TABLE_DATA[DIM_X];   /* f(end) */
    }
    else
    {
        /* input within boundaries -> interpolation */
        /** - calculate table index */
        idxx = (uint16)inx;            /*  x */
        
        /** - calculate fractual part */
        deltax = inx - idxx;            /* dx */
        
        /** - calculate value at lattice points */
        fact  = TABLE_DATA[idxx];       /* f(x) */
        factx = TABLE_DATA[idxx + 1];   /* f(x+1) */

        /** - linear interpolation */
        tmp = (factx - fact) * deltax;  /* dx*(f(x+1) - f(x)) */
        tmp = fact + tmp;               /* f(x) + dx*(f(x+1) - f(x)) */
    }

    OUT = tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable1D_Float32_Init(LOOKUPTABLE1D_FLOAT32 *pTLookupTable1D_Float32)
{
    pTLookupTable1D_Float32->ID = LOOKUPTABLE1D_FLOAT32_ID;
    pTLookupTable1D_Float32->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_Float32_Load(const LOOKUPTABLE1D_FLOAT32 *pTLookupTable1D_Float32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 10U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(pTLookupTable1D_Float32->dimX & 0xFFU);
        data[1] = (uint8)((pTLookupTable1D_Float32->dimX >> 8) & 0xFFU);
        data[2] = (uint8)(*(uint32*)&pTLookupTable1D_Float32->gainX & 0xFFU);
        data[3] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->gainX >> 8) & 0xFFU);
        data[4] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->gainX >> 16) & 0xFFU);
        data[5] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->gainX >> 24) & 0xFFU);
        data[6] = (uint8)(*(uint32*)&pTLookupTable1D_Float32->offsetX & 0xFFU);
        data[7] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->offsetX >> 8) & 0xFFU);
        data[8] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->offsetX >> 16) & 0xFFU);
        data[9] = (uint8)((*(uint32*)&pTLookupTable1D_Float32->offsetX >> 24) & 0xFFU);
        *dataLength = 10U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable1D_Float32_Save(LOOKUPTABLE1D_FLOAT32 *pTLookupTable1D_Float32, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    uint32 tmp32;

    if (dataLength != 10U)
    {
        error = 1U;
    }
    else
    {
        pTLookupTable1D_Float32->dimX = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        tmp32 = (uint32)((uint32)data[2] | ((uint32)data[3] << 8) | ((uint32)data[4] << 16) | ((uint32)data[5] << 24));
        pTLookupTable1D_Float32->gainX = *(float32*)&tmp32;
        tmp32 = (uint32)((uint32)data[6] | ((uint32)data[7] << 8) | ((uint32)data[8] << 16) | ((uint32)data[9] << 24));
        pTLookupTable1D_Float32->offsetX = *(float32*)&tmp32;
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE1D_FLOAT32_ISLINKED)
void* LookupTable1D_Float32_GetAddress(const LOOKUPTABLE1D_FLOAT32* block, uint16 elementId)
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
