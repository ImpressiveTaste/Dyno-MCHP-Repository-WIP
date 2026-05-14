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
 * $LastChangedRevision: 2077 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: Two dimensional look-up table with selectable number of entries */
/* USERCODE-END:Description                                                                                           */
#include "LookupTable2D_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable2D_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*pTLookupTable2D_FiP32->x)
#define IN_Y            (*pTLookupTable2D_FiP32->y)

/* Outputs */
#define OUT             (pTLookupTable2D_FiP32->Out)

/* Parameters */
#define TABLE_DATA      (pTLookupTable2D_FiP32->Table)
#define SIZE_X          (pTLookupTable2D_FiP32->sizeX)

#define SFR_X           (pTLookupTable2D_FiP32->sfrX)
#define MASK_X          (pTLookupTable2D_FiP32->maskX)
#define IDX_OFFSET_X    (pTLookupTable2D_FiP32->idxOffsetX)
#define MIN_X           (pTLookupTable2D_FiP32->minX)
#define MAX_X           (pTLookupTable2D_FiP32->maxX)
#define GAIN_X          (pTLookupTable2D_FiP32->gainX)
#define GAIN_X_SFR      (pTLookupTable2D_FiP32->gainXsfr)
#define OFFSET_X        (pTLookupTable2D_FiP32->offsetX)

#define SFR_Y           (pTLookupTable2D_FiP32->sfrY)
#define MASK_Y          (pTLookupTable2D_FiP32->maskY)
#define IDX_OFFSET_Y    (pTLookupTable2D_FiP32->idxOffsetY)
#define MIN_Y           (pTLookupTable2D_FiP32->minY)
#define MAX_Y           (pTLookupTable2D_FiP32->maxY)
#define GAIN_Y          (pTLookupTable2D_FiP32->gainY)
#define GAIN_Y_SFR      (pTLookupTable2D_FiP32->gainYsfr)
#define OFFSET_Y        (pTLookupTable2D_FiP32->offsetY)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable2D_FiP32_Update(LOOKUPTABLE2D_FIP32 *pTLookupTable2D_FiP32)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 tmp;
    uint16 idxx, idxy, idx;
    int64 fact, factx, facty, factxy, deltax, deltay, deltaxy;
    int32 inx, iny;
    
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
    if (IN_Y < MIN_Y)
    {
        /* input below lower boundary -> cutoff */
        iny = -INT32_MAX;
    }
    else if (IN_Y > MAX_Y)
    {
        /* input above higher boundary -> cutoff */
        iny = INT32_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int64)IN_Y - (int64)OFFSET_Y);
        iny = ((int64)tmp * (int64)GAIN_Y) >> GAIN_Y_SFR;
    }
    
    /** - calculate table indexes */
    idxx   = (inx >> SFR_X) + IDX_OFFSET_X;    /*  x  */
    idxy   = (iny >> SFR_Y) + IDX_OFFSET_Y;    /*  y  */
    idx    = idxx + (idxy*SIZE_X);
    
    /** - calculate fractual part */
    deltax  = inx & MASK_X;                                     /* dx  */
    deltay  = iny & MASK_Y;                                     /* dy  */
    deltaxy = ((int64)(deltax)*(int64)(deltay)) >> SFR_Y;       /* dxy */

    /** - calculate value at lattice points */
    fact   = TABLE_DATA[idx];                                   /* f(x  ,y)   */
    factx  = TABLE_DATA[idx + 1];                               /* f(x+1,y)   */
    facty  = TABLE_DATA[idx + SIZE_X];                          /* f(x  ,y+1) */
    factxy = TABLE_DATA[idx + SIZE_X+1];                        /* f(x+1,y+1) */

    /** - bilinear interpolation */
    tmp  = fact;                                                /* f(x,y) */
    tmp += (((int64)(factx - fact) * (int64)deltax) >> SFR_X);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) */
    tmp += (((int64)(facty - fact) * (int64)deltay) >> SFR_Y);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))*/
    tmp += ((((int64)fact + (int64)factxy - (int64)factx - (int64)facty) * (int64)deltaxy) >> SFR_X);   /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))  + dxy*(f(x,y) + f(x+1,y+1) - f(x+1,y) - f(x,y+1)) */

    OUT = (int32)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable2D_FiP32_Init(LOOKUPTABLE2D_FIP32 *pTLookupTable2D_FiP32)
{
    pTLookupTable2D_FiP32->ID = LOOKUPTABLE2D_FIP32_ID;
    pTLookupTable2D_FiP32->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP32_Load(const LOOKUPTABLE2D_FIP32 *pTLookupTable2D_FiP32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)50 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)pTLookupTable2D_FiP32->sfrX;
        data[1] = (uint8)(pTLookupTable2D_FiP32->maskX & 0x000000FF);
        data[2] = (uint8)((pTLookupTable2D_FiP32->maskX >> 8) & 0x000000FF);
        data[3] = (uint8)((pTLookupTable2D_FiP32->maskX >> 16) & 0x000000FF);
        data[4] = (uint8)((pTLookupTable2D_FiP32->maskX >> 24) & 0x000000FF);
        data[5] = (uint8)(pTLookupTable2D_FiP32->idxOffsetX & 0x00FF);
        data[6] = (uint8)((pTLookupTable2D_FiP32->idxOffsetX >> 8) & 0x00FF);
        data[7] = (uint8)(pTLookupTable2D_FiP32->sizeX & 0x00FF);
        data[8] = (uint8)((pTLookupTable2D_FiP32->sizeX >> 8) & 0x00FF);
        data[9] = (uint8)(pTLookupTable2D_FiP32->gainX & 0x000000FF);
        data[10] = (uint8)((pTLookupTable2D_FiP32->gainX >> 8) & 0x000000FF);
        data[11] = (uint8)((pTLookupTable2D_FiP32->gainX >> 16) & 0x000000FF);
        data[12] = (uint8)((pTLookupTable2D_FiP32->gainX >> 24) & 0x000000FF);
        data[13] = (uint8)pTLookupTable2D_FiP32->gainXsfr;
        data[14] = (uint8)(pTLookupTable2D_FiP32->offsetX & 0x000000FF);
        data[15] = (uint8)((pTLookupTable2D_FiP32->offsetX >> 8) & 0x000000FF);
        data[16] = (uint8)((pTLookupTable2D_FiP32->offsetX >> 16) & 0x000000FF);
        data[17] = (uint8)((pTLookupTable2D_FiP32->offsetX >> 24) & 0x000000FF);
        data[18] = (uint8)(pTLookupTable2D_FiP32->minX & 0x000000FF);
        data[19] = (uint8)((pTLookupTable2D_FiP32->minX >> 8) & 0x000000FF);
        data[20] = (uint8)((pTLookupTable2D_FiP32->minX >> 16) & 0x000000FF);
        data[21] = (uint8)((pTLookupTable2D_FiP32->minX >> 24) & 0x000000FF);
        data[22] = (uint8)(pTLookupTable2D_FiP32->maxX & 0x000000FF);
        data[23] = (uint8)((pTLookupTable2D_FiP32->maxX >> 8) & 0x000000FF);
        data[24] = (uint8)((pTLookupTable2D_FiP32->maxX >> 16) & 0x000000FF);
        data[25] = (uint8)((pTLookupTable2D_FiP32->maxX >> 24) & 0x000000FF);
        data[26] = (uint8)pTLookupTable2D_FiP32->sfrY;
        data[27] = (uint8)(pTLookupTable2D_FiP32->maskY & 0x000000FF);
        data[28] = (uint8)((pTLookupTable2D_FiP32->maskY >> 8) & 0x000000FF);
        data[29] = (uint8)((pTLookupTable2D_FiP32->maskY >> 16) & 0x000000FF);
        data[30] = (uint8)((pTLookupTable2D_FiP32->maskY >> 24) & 0x000000FF);
        data[31] = (uint8)(pTLookupTable2D_FiP32->idxOffsetY & 0x00FF);
        data[32] = (uint8)((pTLookupTable2D_FiP32->idxOffsetY >> 8) & 0x00FF);
        data[33] = (uint8)(pTLookupTable2D_FiP32->gainY & 0x000000FF);
        data[34] = (uint8)((pTLookupTable2D_FiP32->gainY >> 8) & 0x000000FF);
        data[35] = (uint8)((pTLookupTable2D_FiP32->gainY >> 16) & 0x000000FF);
        data[36] = (uint8)((pTLookupTable2D_FiP32->gainY >> 24) & 0x000000FF);
        data[37] = (uint8)pTLookupTable2D_FiP32->gainYsfr;
        data[38] = (uint8)(pTLookupTable2D_FiP32->offsetY & 0x000000FF);
        data[39] = (uint8)((pTLookupTable2D_FiP32->offsetY >> 8) & 0x000000FF);
        data[40] = (uint8)((pTLookupTable2D_FiP32->offsetY >> 16) & 0x000000FF);
        data[41] = (uint8)((pTLookupTable2D_FiP32->offsetY >> 24) & 0x000000FF);
        data[42] = (uint8)(pTLookupTable2D_FiP32->minY & 0x000000FF);
        data[43] = (uint8)((pTLookupTable2D_FiP32->minY >> 8) & 0x000000FF);
        data[44] = (uint8)((pTLookupTable2D_FiP32->minY >> 16) & 0x000000FF);
        data[45] = (uint8)((pTLookupTable2D_FiP32->minY >> 24) & 0x000000FF);
        data[46] = (uint8)(pTLookupTable2D_FiP32->maxY & 0x000000FF);
        data[47] = (uint8)((pTLookupTable2D_FiP32->maxY >> 8) & 0x000000FF);
        data[48] = (uint8)((pTLookupTable2D_FiP32->maxY >> 16) & 0x000000FF);
        data[49] = (uint8)((pTLookupTable2D_FiP32->maxY >> 24) & 0x000000FF);
        *dataLength = (uint16)50;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP32_Save(LOOKUPTABLE2D_FIP32 *pTLookupTable2D_FiP32, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)50)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable2D_FiP32->sfrX = UINT8_TO_INT8((uint8)data[0]);
        pTLookupTable2D_FiP32->maskX = ((uint32)data[1] + \
            ((uint32)data[2] << 8) + ((uint32)data[3] << 16) + \
            ((uint32)data[4] << 24));
        pTLookupTable2D_FiP32->idxOffsetX = ((uint16)data[5] + \
            ((uint16)data[6] << 8));
        pTLookupTable2D_FiP32->sizeX = ((uint16)data[7] + \
            ((uint16)data[8] << 8));
        pTLookupTable2D_FiP32->gainX = UINT32_TO_INT32((uint32)data[9] + \
            ((uint32)data[10] << 8) + ((uint32)data[11] << 16) + \
            ((uint32)data[12] << 24));
        pTLookupTable2D_FiP32->gainXsfr = UINT8_TO_INT8((uint8)data[13]);
        pTLookupTable2D_FiP32->offsetX = UINT32_TO_INT32((uint32)data[14] + \
            ((uint32)data[15] << 8) + ((uint32)data[16] << 16) + \
            ((uint32)data[17] << 24));
        pTLookupTable2D_FiP32->minX = UINT32_TO_INT32((uint32)data[18] + \
            ((uint32)data[19] << 8) + ((uint32)data[20] << 16) + \
            ((uint32)data[21] << 24));
        pTLookupTable2D_FiP32->maxX = UINT32_TO_INT32((uint32)data[22] + \
            ((uint32)data[23] << 8) + ((uint32)data[24] << 16) + \
            ((uint32)data[25] << 24));
        pTLookupTable2D_FiP32->sfrY = UINT8_TO_INT8((uint8)data[26]);
        pTLookupTable2D_FiP32->maskY = ((uint32)data[27] + \
            ((uint32)data[28] << 8) + ((uint32)data[29] << 16) + \
            ((uint32)data[30] << 24));
        pTLookupTable2D_FiP32->idxOffsetY = ((uint16)data[31] + \
            ((uint16)data[32] << 8));
        pTLookupTable2D_FiP32->gainY = UINT32_TO_INT32((uint32)data[33] + \
            ((uint32)data[34] << 8) + ((uint32)data[35] << 16) + \
            ((uint32)data[36] << 24));
        pTLookupTable2D_FiP32->gainYsfr = UINT8_TO_INT8((uint8)data[37]);
        pTLookupTable2D_FiP32->offsetY = UINT32_TO_INT32((uint32)data[38] + \
            ((uint32)data[39] << 8) + ((uint32)data[40] << 16) + \
            ((uint32)data[41] << 24));
        pTLookupTable2D_FiP32->minY = UINT32_TO_INT32((uint32)data[42] + \
            ((uint32)data[43] << 8) + ((uint32)data[44] << 16) + \
            ((uint32)data[45] << 24));
        pTLookupTable2D_FiP32->maxY = UINT32_TO_INT32((uint32)data[46] + \
            ((uint32)data[47] << 8) + ((uint32)data[48] << 16) + \
            ((uint32)data[49] << 24));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE2D_FIP32_ISLINKED)
void* LookupTable2D_FiP32_GetAddress(const LOOKUPTABLE2D_FIP32* block, uint16 elementId)
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
