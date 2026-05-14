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
 * $LastChangedRevision: 2543 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: Two dimensional look-up table with selectable number of entries */
/* USERCODE-END:Description                                                                                           */
#include "LookupTable2D_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable2D_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*pTLookupTable2D_Float64->x)
#define IN_Y        (*pTLookupTable2D_Float64->y)

/* Outputs */
#define OUT         (pTLookupTable2D_Float64->Out)

/* Parameters */
#define TABLE_DATA   (pTLookupTable2D_Float64->Table)

#define DIM_X        (pTLookupTable2D_Float64->dimX)
#define GAIN_X       (pTLookupTable2D_Float64->gainX)
#define OFFSET_X     (pTLookupTable2D_Float64->offsetX)

#define DIM_Y        (pTLookupTable2D_Float64->dimY)
#define GAIN_Y       (pTLookupTable2D_Float64->gainY)
#define OFFSET_Y     (pTLookupTable2D_Float64->offsetY)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable2D_Float64_Update(LOOKUPTABLE2D_FLOAT64 *pTLookupTable2D_Float64)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 tmp;
    uint16 idxx, idxy, idx;
    float64 fact, factx, facty, factxy, deltax, deltay, deltaxy;
    float64 inx, iny;
    
    /** - scale inputs */
    inx = (IN_X - OFFSET_X) * GAIN_X;
    iny = (IN_Y - OFFSET_Y) * GAIN_Y;

    /** - calculate table indexes */
    idxx = (uint16)inx;         /*  x  */
    idxy = (uint16)iny;         /*  y  */

    /** - calculate fractual part */
    deltax = inx - idxx;        /* dx  */
    deltay = iny - idxy;        /* dy  */
    
    /** - check boundaries and adjust indexes and fractual parts, if necessary */
    if (inx < 0)
    {
        /* index below lower boundary -> cutoff */
        idxx = 0;
        deltax = 0; /* cut-off mode*/
    }
    else if ((uint16)inx >= DIM_X)
    {
        /* index above higher boundary -> cutoff */
        idxx = DIM_X;
        deltax = 0;
    }
    if (iny < 0)
    {
        /* index below lower boundary -> cutoff */
        idxy = 0;
        deltay = 0; /* cut-off mode*/
    }
    else if ((uint16)iny >= DIM_Y)
    {
        /* index above higher boundary -> cutoff */
        idxy = DIM_Y;
        deltay = 0;
    }
    idx     = idxx + (idxy*(DIM_X+1));
    deltaxy = deltax * deltay;   /* dxy */

    /** - calculate value at lattice points */
    fact   = TABLE_DATA[idx];                           /* f(x  ,y)   */
    factx  = TABLE_DATA[idx + 1];                       /* f(x+1,y)   */
    facty  = TABLE_DATA[idx + DIM_X+1];                 /* f(x  ,y+1) */
    factxy = TABLE_DATA[idx + DIM_X+2];                 /* f(x+1,y+1) */

    /** - bilinear interpolation */
    tmp  = fact;                                        /* f(x,y) */
    tmp += ((factx - fact) * deltax);                   /* f(x,y) + dx*(f(x+1,y) - f(x,y)) */
    tmp += ((facty - fact) * deltay);                   /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y)) */
    tmp += ((fact + factxy - factx - facty) * deltaxy); /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))  + dxy*(f(x,y) + f(x+1,y+1) - f(x+1,y) - f(x,y+1)) */

    OUT = tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable2D_Float64_Init(LOOKUPTABLE2D_FLOAT64 *pTLookupTable2D_Float64)
{
    pTLookupTable2D_Float64->ID = LOOKUPTABLE2D_FLOAT64_ID;
    pTLookupTable2D_Float64->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_Float64_Load(const LOOKUPTABLE2D_FLOAT64 *pTLookupTable2D_Float64, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)68 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTLookupTable2D_Float64->dimX & 0x00FF);
        data[1] = (uint8)((pTLookupTable2D_Float64->dimX >> 8) & 0x00FF);
        data[2] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX)) & 0x00000000000000FF);
        data[3] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 8) & 0x00000000000000FF);
        data[4] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 16) & 0x00000000000000FF);
        data[5] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 24) & 0x00000000000000FF);
        data[6] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 32) & 0x00000000000000FF);
        data[7] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 40) & 0x00000000000000FF);
        data[8] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 48) & 0x00000000000000FF);
        data[9] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainX) >> 56) & 0x00000000000000FF);
        data[10] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX)) & 0x00000000000000FF);
        data[11] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 8) & 0x00000000000000FF);
        data[12] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 16) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 24) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 32) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 40) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 48) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetX) >> 56) & 0x00000000000000FF);
        data[18] = (uint8)(pTLookupTable2D_Float64->dimY & 0x00FF);
        data[19] = (uint8)((pTLookupTable2D_Float64->dimY >> 8) & 0x00FF);
        data[20] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY)) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 8) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 16) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 24) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 32) & 0x00000000000000FF);
        data[25] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 40) & 0x00000000000000FF);
        data[26] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 48) & 0x00000000000000FF);
        data[27] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->gainY) >> 56) & 0x00000000000000FF);
        data[28] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY)) & 0x00000000000000FF);
        data[29] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 8) & 0x00000000000000FF);
        data[30] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 16) & 0x00000000000000FF);
        data[31] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 24) & 0x00000000000000FF);
        data[32] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 32) & 0x00000000000000FF);
        data[33] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 40) & 0x00000000000000FF);
        data[34] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 48) & 0x00000000000000FF);
        data[35] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->offsetY) >> 56) & 0x00000000000000FF);
        data[36] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX)) & 0x00000000000000FF);
        data[37] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 8) & 0x00000000000000FF);
        data[38] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 16) & 0x00000000000000FF);
        data[39] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 24) & 0x00000000000000FF);
        data[40] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 32) & 0x00000000000000FF);
        data[41] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 40) & 0x00000000000000FF);
        data[42] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 48) & 0x00000000000000FF);
        data[43] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minX) >> 56) & 0x00000000000000FF);
        data[44] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX)) & 0x00000000000000FF);
        data[45] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 8) & 0x00000000000000FF);
        data[46] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 16) & 0x00000000000000FF);
        data[47] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 24) & 0x00000000000000FF);
        data[48] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 32) & 0x00000000000000FF);
        data[49] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 40) & 0x00000000000000FF);
        data[50] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 48) & 0x00000000000000FF);
        data[51] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxX) >> 56) & 0x00000000000000FF);
        data[52] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY)) & 0x00000000000000FF);
        data[53] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 8) & 0x00000000000000FF);
        data[54] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 16) & 0x00000000000000FF);
        data[55] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 24) & 0x00000000000000FF);
        data[56] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 32) & 0x00000000000000FF);
        data[57] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 40) & 0x00000000000000FF);
        data[58] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 48) & 0x00000000000000FF);
        data[59] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->minY) >> 56) & 0x00000000000000FF);
        data[60] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY)) & 0x00000000000000FF);
        data[61] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 8) & 0x00000000000000FF);
        data[62] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 16) & 0x00000000000000FF);
        data[63] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 24) & 0x00000000000000FF);
        data[64] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 32) & 0x00000000000000FF);
        data[65] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 40) & 0x00000000000000FF);
        data[66] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 48) & 0x00000000000000FF);
        data[67] = (uint8)((*(uint64*)&(pTLookupTable2D_Float64->maxY) >> 56) & 0x00000000000000FF);
        *dataLength = (uint16)68;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_Float64_Save(LOOKUPTABLE2D_FLOAT64 *pTLookupTable2D_Float64, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    uint64 tmp64;

    if (dataLength != (uint16)68)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable2D_Float64->dimX = ((uint16)data[0] + \
            ((uint16)data[1] << 8));
        tmp64 = (uint64)data[2] + \
            ((uint64)data[3] << 8) + ((uint64)data[4] << 16) + \
            ((uint64)data[5] << 24) + ((uint64)data[6] << 32) + \
            ((uint64)data[7] << 40) + ((uint64)data[8] << 48) + \
            ((uint64)data[9] << 56);
        pTLookupTable2D_Float64->gainX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[10] + \
            ((uint64)data[11] << 8) + ((uint64)data[12] << 16) + \
            ((uint64)data[13] << 24) + ((uint64)data[14] << 32) + \
            ((uint64)data[15] << 40) + ((uint64)data[16] << 48) + \
            ((uint64)data[17] << 56);
        pTLookupTable2D_Float64->offsetX = (float64)(*(float64*)&tmp64);
        pTLookupTable2D_Float64->dimY = ((uint16)data[18] + \
            ((uint16)data[19] << 8));
        tmp64 = (uint64)data[20] + \
            ((uint64)data[21] << 8) + ((uint64)data[22] << 16) + \
            ((uint64)data[23] << 24) + ((uint64)data[24] << 32) + \
            ((uint64)data[25] << 40) + ((uint64)data[26] << 48) + \
            ((uint64)data[27] << 56);
        pTLookupTable2D_Float64->gainY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[28] + \
            ((uint64)data[29] << 8) + ((uint64)data[30] << 16) + \
            ((uint64)data[31] << 24) + ((uint64)data[32] << 32) + \
            ((uint64)data[33] << 40) + ((uint64)data[34] << 48) + \
            ((uint64)data[35] << 56);
        pTLookupTable2D_Float64->offsetY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[36] + \
            ((uint64)data[37] << 8) + ((uint64)data[38] << 16) + \
            ((uint64)data[39] << 24) + ((uint64)data[40] << 32) + \
            ((uint64)data[41] << 40) + ((uint64)data[42] << 48) + \
            ((uint64)data[43] << 56);
        pTLookupTable2D_Float64->minX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[44] + \
            ((uint64)data[45] << 8) + ((uint64)data[46] << 16) + \
            ((uint64)data[47] << 24) + ((uint64)data[48] << 32) + \
            ((uint64)data[49] << 40) + ((uint64)data[50] << 48) + \
            ((uint64)data[51] << 56);
        pTLookupTable2D_Float64->maxX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[52] + \
            ((uint64)data[53] << 8) + ((uint64)data[54] << 16) + \
            ((uint64)data[55] << 24) + ((uint64)data[56] << 32) + \
            ((uint64)data[57] << 40) + ((uint64)data[58] << 48) + \
            ((uint64)data[59] << 56);
        pTLookupTable2D_Float64->minY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[60] + \
            ((uint64)data[61] << 8) + ((uint64)data[62] << 16) + \
            ((uint64)data[63] << 24) + ((uint64)data[64] << 32) + \
            ((uint64)data[65] << 40) + ((uint64)data[66] << 48) + \
            ((uint64)data[67] << 56);
        pTLookupTable2D_Float64->maxY = (float64)(*(float64*)&tmp64);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE2D_FLOAT64_ISLINKED)
void* LookupTable2D_Float64_GetAddress(const LOOKUPTABLE2D_FLOAT64* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = (void*)block->x;
            break;
        case 2:
            addr = (void*)block->y;
            break;
        case 3:
            addr = (void*)&block->Out;
            break;
        default:
            addr = (void*)0;
            break;
    }
    return (addr);
}
#endif
