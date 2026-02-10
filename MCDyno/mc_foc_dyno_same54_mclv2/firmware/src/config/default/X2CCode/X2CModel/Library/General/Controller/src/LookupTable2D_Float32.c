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
/* Description: Two dimensional look-up table with selectable number of entries */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "LookupTable2D_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable2D_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*block->x)
#define IN_Y        (*block->y)

/* Outputs */
#define OUT         (block->Out)

/* Parameters */
#define TABLE_DATA  (block->Table)

#define DIM_X       (block->dimX)
#define GAIN_X      (block->gainX)
#define OFFSET_X    (block->offsetX)
#define MIN_X       (block->minX)
#define MAX_X       (block->maxX)

#define GAIN_Y      (block->gainY)
#define OFFSET_Y    (block->offsetY)
#define MIN_Y       (block->minY)
#define MAX_Y       (block->maxY)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable2D_Float32_Update(LOOKUPTABLE2D_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 tmp;
    uint16 idxx, idxy, idx;
    float32 fact, factx, facty, factxy, deltax, deltay, deltaxy;
    float32 inx, iny;

    /** - limit inputs */
    if (IN_X < MIN_X)
    {
        inx = MIN_X;
    }
    else if (IN_X > MAX_X)
    {
        inx = MAX_X;
    }
    else
    {
        inx = IN_X;
    }

    if (IN_Y < MIN_Y)
    {
        iny = MIN_Y;
    }
    else if (IN_Y > MAX_Y)
    {
        iny = MAX_Y;
    }
    else
    {
        iny = IN_Y;
    }

    /** - scale inputs */
    inx = (inx - OFFSET_X) * GAIN_X;
    iny = (iny - OFFSET_Y) * GAIN_Y;

    /** - calculate table indexes */
    idxx = (uint16)inx;         /*  x  */
    idxy = (uint16)iny;         /*  y  */
    idx  = idxx + (idxy*(DIM_X+1));

    /** - calculate fractual part */
    deltax = inx - idxx;        /* dx  */
    deltay = iny - idxy;        /* dy  */
    deltaxy = deltax * deltay;  /* dxy */

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
void LookupTable2D_Float32_Init(LOOKUPTABLE2D_FLOAT32 *block)
{
    block->ID = LOOKUPTABLE2D_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_Float32_Load(const LOOKUPTABLE2D_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 36U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        data[0] = (uint8)(block->dimX & 0xFFU);
        data[1] = (uint8)((block->dimX >> 8) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->gainX, sizeof(uint32));
        data[2] = (uint8)(tmp32 & 0xFFU);
        data[3] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[4] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[5] = (uint8)((tmp32 >> 24) & 0xFFU);
        data[6] = (uint8)(block->dimY & 0xFFU);
        data[7] = (uint8)((block->dimY >> 8) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->gainY, sizeof(uint32));
        data[8] = (uint8)(tmp32 & 0xFFU);
        data[9] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offsetX, sizeof(uint32));
        data[12] = (uint8)(tmp32 & 0xFFU);
        data[13] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[14] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[15] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offsetY, sizeof(uint32));
        data[16] = (uint8)(tmp32 & 0xFFU);
        data[17] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->minX, sizeof(uint32));
        data[20] = (uint8)(tmp32 & 0xFFU);
        data[21] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[22] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[23] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->maxX, sizeof(uint32));
        data[24] = (uint8)(tmp32 & 0xFFU);
        data[25] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[26] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[27] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->minY, sizeof(uint32));
        data[28] = (uint8)(tmp32 & 0xFFU);
        data[29] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[30] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[31] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->maxY, sizeof(uint32));
        data[32] = (uint8)(tmp32 & 0xFFU);
        data[33] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[34] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[35] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 36U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_Float32_Save(LOOKUPTABLE2D_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 36U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        block->dimX = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        tmp32 = (uint32)((uint32)data[2] | ((uint32)data[3] << 8) | ((uint32)data[4] << 16) | ((uint32)data[5] << 24));
        (void) memcpy((void*)&block->gainX, (const void*)&tmp32, sizeof(float32));
        block->dimY = (uint16)((uint16)data[6] | ((uint16)data[7] << 8));
        tmp32 = (uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24));
        (void) memcpy((void*)&block->gainY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24));
        (void) memcpy((void*)&block->offsetX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24));
        (void) memcpy((void*)&block->offsetY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[20] | ((uint32)data[21] << 8) | ((uint32)data[22] << 16) | ((uint32)data[23] << 24));
        (void) memcpy((void*)&block->minX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[24] | ((uint32)data[25] << 8) | ((uint32)data[26] << 16) | ((uint32)data[27] << 24));
        (void) memcpy((void*)&block->maxX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[28] | ((uint32)data[29] << 8) | ((uint32)data[30] << 16) | ((uint32)data[31] << 24));
        (void) memcpy((void*)&block->minY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[32] | ((uint32)data[33] << 8) | ((uint32)data[34] << 16) | ((uint32)data[35] << 24));
        (void) memcpy((void*)&block->maxY, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE2D_FLOAT32_ISLINKED)
void* LookupTable2D_Float32_GetAddress(LOOKUPTABLE2D_FLOAT32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->x;
            break;
        case 2:
            addr = block->y;
            break;
        case 3:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
