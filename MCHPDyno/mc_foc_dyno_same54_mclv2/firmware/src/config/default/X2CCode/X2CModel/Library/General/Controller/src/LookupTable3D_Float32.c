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
/* Description: Three dimensional look-up table with selectable number of entries and trilinear interpolation
 *
 * Calculation:
 *              f(x,y,z) = c0 + c1*dx + c2*dy + c3*dz + c4*dx*dy + c5*dy*dz + c6*dx*dz + c7*dx*dy*dz
 *        with
 *              c0 = f(x0,y0,z0)
 *              c1 = f(x1,y0,z0) - f(x0,y0,z0)
 *              c2 = f(x0,y1,z0) - f(x0,y0,z0)
 *              c3 = f(x0,y0,z1) - f(x0,y0,z0)
 *              c4 = f(x1,y1,z0) - f(x0,y1,z0) - f(x1,y0,z0) + f(x0,y0,z0)
 *              c5 = f(x0,y1,z1) - f(x0,y0,z1) - f(x0,y1,z0) + f(x0,y0,z0)
 *              c6 = f(x1,y0,z1) - f(x0,y0,z1) - f(x1,y0,z0) + f(x0,y0,z0)
 *              c4 = f(x1,y1,z1) - f(x0,y1,z1) - f(x1,y0,z1) - f(x1,y1,z0) + f(x1,y0,z0) + f(x0,y0,z1) + f(x0,y1,z0) - f(x0,y0,z0)
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "LookupTable3D_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*block->x)
#define IN_Y        (*block->y)
#define IN_Z        (*block->z)

/* Outputs */
#define OUT         (block->Out)

/* Parameters */
#define TABLE_DATA  (block->Table)
#define SIZE_X      (block->sizeX)
#define SIZE_XY     (block->sizeXY)

#define GAIN_X      (block->gainX)
#define OFFSET_X    (block->offsetX)
#define MIN_X       (block->minX)
#define MAX_X       (block->maxX)

#define GAIN_Y      (block->gainY)
#define OFFSET_Y    (block->offsetY)
#define MIN_Y       (block->minY)
#define MAX_Y       (block->maxY)

#define GAIN_Z      (block->gainZ)
#define OFFSET_Z    (block->offsetZ)
#define MIN_Z       (block->minZ)
#define MAX_Z       (block->maxZ)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_Float32_Update(LOOKUPTABLE3D_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    uint16 idx, idxx, idxy, idxz, idx1;
    float32 dx, dy, dz;
    float32 f000, f001, f010, f011, f100, f101, f110, f111;
    float32 c0, c1, c2, c3, c4, c5, c6, c7;
    float32 inx, iny, inz;

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

    if (IN_Z < MIN_Z)
    {
        inz = MIN_Z;
    }
    else if (IN_Z > MAX_Z)
    {
        inz = MAX_Z;
    }
    else
    {
        inz = IN_Z;
    }

    /** - scale inputs */
    inx = (inx - OFFSET_X) * GAIN_X;
    iny = (iny - OFFSET_Y) * GAIN_Y;
    inz = (inz - OFFSET_Z) * GAIN_Z;

    /** - calculate table indexes */
    idxx = (uint16)inx;         /*  x  */
    idxy = (uint16)iny;         /*  y  */
    idxz = (uint16)inz;         /*  z  */

    /** - calculate fractual part */
    dx = inx - idxx;            /* dx  */
    dy = iny - idxy;            /* dy  */
    dz = inz - idxz;            /* dy  */

    /** - calculate index of lattice point zero */
    idx  = idxx + (idxy*SIZE_X) + (idxz*SIZE_XY);

    /* simplification for run-time optimization */
    idx1 = idx + 1;

    /** - calculate value at lattice points */
    f000 = TABLE_DATA[idx];                     /* f(x  ,y  ,z  ) */
    f100 = TABLE_DATA[idx1];                    /* f(x+1,y  ,z  ) */
    f010 = TABLE_DATA[idx  + SIZE_X];           /* f(x  ,y+1,z  ) */
    f110 = TABLE_DATA[idx1 + SIZE_X];           /* f(x+1,y+1,z  ) */
    f001 = TABLE_DATA[idx  +          SIZE_XY]; /* f(x  ,y  ,z+1) */
    f101 = TABLE_DATA[idx1 +          SIZE_XY]; /* f(x+1,y  ,z+1) */
    f011 = TABLE_DATA[idx  + SIZE_X + SIZE_XY]; /* f(x  ,y+1,z+1) */
    f111 = TABLE_DATA[idx1 + SIZE_X + SIZE_XY]; /* f(x+1,y+1,z+1) */

    /** - calculate coefficients for interpolation */
    c0 = f000;
    c1 = f100 - f000;
    c2 = f010 - f000;
    c3 = f001 - f000;
    c4 = f110 - f010 - f100 + f000;
    c5 = f011 - f001 - f010 + f000;
    c6 = f101 - f001 - f100 + f000;
    c7 = f111 - f011 - f101 - f110 + f100 + f001 + f010 - f000;

    /** - trilinear interpolation */
    OUT = c0 + c1*dx + c2*dy + c3*dz + c4*dx*dy + c5*dy*dz + c6*dx*dz + c7*dx*dy*dz;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable3D_Float32_Init(LOOKUPTABLE3D_FLOAT32 *block)
{
    block->ID = LOOKUPTABLE3D_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float32_Load(const LOOKUPTABLE3D_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 60U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        data[0] = (uint8)(block->dimX & 0xFFU);
        data[1] = (uint8)((block->dimX >> 8) & 0xFFU);
        data[2] = (uint8)(block->dimY & 0xFFU);
        data[3] = (uint8)((block->dimY >> 8) & 0xFFU);
        data[4] = (uint8)(block->dimZ & 0xFFU);
        data[5] = (uint8)((block->dimZ >> 8) & 0xFFU);
        data[6] = (uint8)(block->sizeX & 0xFFU);
        data[7] = (uint8)((block->sizeX >> 8) & 0xFFU);
        data[8] = (uint8)(block->sizeY & 0xFFU);
        data[9] = (uint8)((block->sizeY >> 8) & 0xFFU);
        data[10] = (uint8)(block->sizeXY & 0xFFU);
        data[11] = (uint8)((block->sizeXY >> 8) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->gainX, sizeof(uint32));
        data[12] = (uint8)(tmp32 & 0xFFU);
        data[13] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[14] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[15] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->gainY, sizeof(uint32));
        data[16] = (uint8)(tmp32 & 0xFFU);
        data[17] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->gainZ, sizeof(uint32));
        data[20] = (uint8)(tmp32 & 0xFFU);
        data[21] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[22] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[23] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offsetX, sizeof(uint32));
        data[24] = (uint8)(tmp32 & 0xFFU);
        data[25] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[26] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[27] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offsetY, sizeof(uint32));
        data[28] = (uint8)(tmp32 & 0xFFU);
        data[29] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[30] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[31] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offsetZ, sizeof(uint32));
        data[32] = (uint8)(tmp32 & 0xFFU);
        data[33] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[34] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[35] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->minX, sizeof(uint32));
        data[36] = (uint8)(tmp32 & 0xFFU);
        data[37] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[38] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[39] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->maxX, sizeof(uint32));
        data[40] = (uint8)(tmp32 & 0xFFU);
        data[41] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[42] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[43] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->minY, sizeof(uint32));
        data[44] = (uint8)(tmp32 & 0xFFU);
        data[45] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[46] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[47] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->maxY, sizeof(uint32));
        data[48] = (uint8)(tmp32 & 0xFFU);
        data[49] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[50] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[51] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->minZ, sizeof(uint32));
        data[52] = (uint8)(tmp32 & 0xFFU);
        data[53] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[54] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[55] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->maxZ, sizeof(uint32));
        data[56] = (uint8)(tmp32 & 0xFFU);
        data[57] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[58] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[59] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 60U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float32_Save(LOOKUPTABLE3D_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 60U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        block->dimX = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        block->dimY = (uint16)((uint16)data[2] | ((uint16)data[3] << 8));
        block->dimZ = (uint16)((uint16)data[4] | ((uint16)data[5] << 8));
        block->sizeX = (uint16)((uint16)data[6] | ((uint16)data[7] << 8));
        block->sizeY = (uint16)((uint16)data[8] | ((uint16)data[9] << 8));
        block->sizeXY = (uint16)((uint16)data[10] | ((uint16)data[11] << 8));
        tmp32 = (uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24));
        (void) memcpy((void*)&block->gainX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24));
        (void) memcpy((void*)&block->gainY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[20] | ((uint32)data[21] << 8) | ((uint32)data[22] << 16) | ((uint32)data[23] << 24));
        (void) memcpy((void*)&block->gainZ, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[24] | ((uint32)data[25] << 8) | ((uint32)data[26] << 16) | ((uint32)data[27] << 24));
        (void) memcpy((void*)&block->offsetX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[28] | ((uint32)data[29] << 8) | ((uint32)data[30] << 16) | ((uint32)data[31] << 24));
        (void) memcpy((void*)&block->offsetY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[32] | ((uint32)data[33] << 8) | ((uint32)data[34] << 16) | ((uint32)data[35] << 24));
        (void) memcpy((void*)&block->offsetZ, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[36] | ((uint32)data[37] << 8) | ((uint32)data[38] << 16) | ((uint32)data[39] << 24));
        (void) memcpy((void*)&block->minX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[40] | ((uint32)data[41] << 8) | ((uint32)data[42] << 16) | ((uint32)data[43] << 24));
        (void) memcpy((void*)&block->maxX, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[44] | ((uint32)data[45] << 8) | ((uint32)data[46] << 16) | ((uint32)data[47] << 24));
        (void) memcpy((void*)&block->minY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[48] | ((uint32)data[49] << 8) | ((uint32)data[50] << 16) | ((uint32)data[51] << 24));
        (void) memcpy((void*)&block->maxY, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[52] | ((uint32)data[53] << 8) | ((uint32)data[54] << 16) | ((uint32)data[55] << 24));
        (void) memcpy((void*)&block->minZ, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[56] | ((uint32)data[57] << 8) | ((uint32)data[58] << 16) | ((uint32)data[59] << 24));
        (void) memcpy((void*)&block->maxZ, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FLOAT32_ISLINKED)
void* LookupTable3D_Float32_GetAddress(LOOKUPTABLE3D_FLOAT32* block, uint16 elementId)
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
            addr = block->z;
            break;
        case 4:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
