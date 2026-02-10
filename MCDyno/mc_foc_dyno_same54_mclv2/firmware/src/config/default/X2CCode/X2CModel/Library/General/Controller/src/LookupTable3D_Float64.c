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
/* Description: Three dimensional look-up table with selectable number of entries and trilinear interpolation */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "LookupTable3D_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_Float64_Update, "ramfuncs")
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

#define DIM_X       (block->dimX)
#define GAIN_X      (block->gainX)
#define OFFSET_X    (block->offsetX)

#define DIM_Y       (block->dimY)
#define GAIN_Y      (block->gainY)
#define OFFSET_Y    (block->offsetY)

#define DIM_Z       (block->dimZ)
#define GAIN_Z      (block->gainZ)
#define OFFSET_Z    (block->offsetZ)


/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_Float64_Update(LOOKUPTABLE3D_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    uint16 idx, idxx, idxy, idxz, idx1;
    float64 dx, dy, dz;
    float64 f000, f001, f010, f011, f100, f101, f110, f111;
    float64 c0, c1, c2, c3, c4, c5, c6, c7;
    float64 inx, iny, inz;

    /** - scale inputs */
    inx = (IN_X - OFFSET_X) * GAIN_X;
    iny = (IN_Y - OFFSET_Y) * GAIN_Y;
    inz = (IN_Z - OFFSET_Z) * GAIN_Z;

    /** - calculate table indexes */
    idxx = (uint16)inx;         /*  x  */
    idxy = (uint16)iny;         /*  y  */
    idxz = (uint16)inz;         /*  z  */

    /** - calculate fractual part */
    dx = inx - idxx;            /* dx  */
    dy = iny - idxy;            /* dy  */
    dz = inz - idxz;            /* dy  */

    /** - check boundaries and adjust indexes and fractual parts, if necessary */
    if (inx < 0)
    {
        /* index below lower boundary -> cutoff */
        idxx = 0;
        dx = 0; /* cut-off mode*/
    }
    else if ((uint16)inx >= DIM_X)
    {
        /* index above higher boundary -> cutoff */
        idxx = DIM_X;
        dx = 0;
    }
    else
    {
        /* else: No operation required */
    }
    if (iny < 0)
    {
        /* index below lower boundary -> cutoff */
        idxy = 0;
        dy = 0; /* cut-off mode*/
    }
    else if ((uint16)iny >= DIM_Y)
    {
        /* index above higher boundary -> cutoff */
        idxy = DIM_Y;
        dy = 0;
    }
    else
    {
        /* else: No operation required */
    }
    if (inz < 0)
    {
        /* index below lower boundary -> cutoff */
        idxz = 0;
        dz = 0; /* cut-off mode*/
    }
    else if ((uint16)inz >= DIM_Z)
    {
        /* index above higher boundary -> cutoff */
        idxz = DIM_Z;
        dz = 0;
    }
    else
    {
        /* else: No operation required */
    }

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
void LookupTable3D_Float64_Init(LOOKUPTABLE3D_FLOAT64 *block)
{
    block->ID = LOOKUPTABLE3D_FLOAT64_ID;
    block->Out = 0.0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float64_Load(const LOOKUPTABLE3D_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 108U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
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
        (void) memcpy((void*)&tmp64, (const void*)&block->gainX, sizeof(uint64));
        data[12] = (uint8)(tmp64 & 0xFFU);
        data[13] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[14] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[15] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[16] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[17] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[18] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[19] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->gainY, sizeof(uint64));
        data[20] = (uint8)(tmp64 & 0xFFU);
        data[21] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[22] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[23] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[24] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[25] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[26] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[27] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->gainZ, sizeof(uint64));
        data[28] = (uint8)(tmp64 & 0xFFU);
        data[29] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[30] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[31] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[32] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[33] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[34] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[35] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->offsetX, sizeof(uint64));
        data[36] = (uint8)(tmp64 & 0xFFU);
        data[37] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[38] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[39] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[40] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[41] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[42] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[43] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->offsetY, sizeof(uint64));
        data[44] = (uint8)(tmp64 & 0xFFU);
        data[45] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[46] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[47] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[48] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[49] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[50] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[51] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->offsetZ, sizeof(uint64));
        data[52] = (uint8)(tmp64 & 0xFFU);
        data[53] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[54] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[55] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[56] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[57] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[58] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[59] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->minX, sizeof(uint64));
        data[60] = (uint8)(tmp64 & 0xFFU);
        data[61] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[62] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[63] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[64] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[65] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[66] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[67] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->maxX, sizeof(uint64));
        data[68] = (uint8)(tmp64 & 0xFFU);
        data[69] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[70] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[71] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[72] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[73] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[74] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[75] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->minY, sizeof(uint64));
        data[76] = (uint8)(tmp64 & 0xFFU);
        data[77] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[78] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[79] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[80] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[81] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[82] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[83] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->maxY, sizeof(uint64));
        data[84] = (uint8)(tmp64 & 0xFFU);
        data[85] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[86] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[87] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[88] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[89] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[90] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[91] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->minZ, sizeof(uint64));
        data[92] = (uint8)(tmp64 & 0xFFU);
        data[93] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[94] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[95] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[96] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[97] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[98] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[99] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->maxZ, sizeof(uint64));
        data[100] = (uint8)(tmp64 & 0xFFU);
        data[101] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[102] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[103] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[104] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[105] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[106] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[107] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 108U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float64_Save(LOOKUPTABLE3D_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 108U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        block->dimX = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        block->dimY = (uint16)((uint16)data[2] | ((uint16)data[3] << 8));
        block->dimZ = (uint16)((uint16)data[4] | ((uint16)data[5] << 8));
        block->sizeX = (uint16)((uint16)data[6] | ((uint16)data[7] << 8));
        block->sizeY = (uint16)((uint16)data[8] | ((uint16)data[9] << 8));
        block->sizeXY = (uint16)((uint16)data[10] | ((uint16)data[11] << 8));
        tmp64 = (uint64)((uint64)data[12] | ((uint64)data[13] << 8) | ((uint64)data[14] << 16) | ((uint64)data[15] << 24) | 
            ((uint64)data[16] << 32) | ((uint64)data[17] << 40) | ((uint64)data[18] << 48) | ((uint64)data[19] << 56));
        (void) memcpy((void*)&block->gainX, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[20] | ((uint64)data[21] << 8) | ((uint64)data[22] << 16) | ((uint64)data[23] << 24) | 
            ((uint64)data[24] << 32) | ((uint64)data[25] << 40) | ((uint64)data[26] << 48) | ((uint64)data[27] << 56));
        (void) memcpy((void*)&block->gainY, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[28] | ((uint64)data[29] << 8) | ((uint64)data[30] << 16) | ((uint64)data[31] << 24) | 
            ((uint64)data[32] << 32) | ((uint64)data[33] << 40) | ((uint64)data[34] << 48) | ((uint64)data[35] << 56));
        (void) memcpy((void*)&block->gainZ, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[36] | ((uint64)data[37] << 8) | ((uint64)data[38] << 16) | ((uint64)data[39] << 24) | 
            ((uint64)data[40] << 32) | ((uint64)data[41] << 40) | ((uint64)data[42] << 48) | ((uint64)data[43] << 56));
        (void) memcpy((void*)&block->offsetX, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[44] | ((uint64)data[45] << 8) | ((uint64)data[46] << 16) | ((uint64)data[47] << 24) | 
            ((uint64)data[48] << 32) | ((uint64)data[49] << 40) | ((uint64)data[50] << 48) | ((uint64)data[51] << 56));
        (void) memcpy((void*)&block->offsetY, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[52] | ((uint64)data[53] << 8) | ((uint64)data[54] << 16) | ((uint64)data[55] << 24) | 
            ((uint64)data[56] << 32) | ((uint64)data[57] << 40) | ((uint64)data[58] << 48) | ((uint64)data[59] << 56));
        (void) memcpy((void*)&block->offsetZ, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[60] | ((uint64)data[61] << 8) | ((uint64)data[62] << 16) | ((uint64)data[63] << 24) | 
            ((uint64)data[64] << 32) | ((uint64)data[65] << 40) | ((uint64)data[66] << 48) | ((uint64)data[67] << 56));
        (void) memcpy((void*)&block->minX, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[68] | ((uint64)data[69] << 8) | ((uint64)data[70] << 16) | ((uint64)data[71] << 24) | 
            ((uint64)data[72] << 32) | ((uint64)data[73] << 40) | ((uint64)data[74] << 48) | ((uint64)data[75] << 56));
        (void) memcpy((void*)&block->maxX, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[76] | ((uint64)data[77] << 8) | ((uint64)data[78] << 16) | ((uint64)data[79] << 24) | 
            ((uint64)data[80] << 32) | ((uint64)data[81] << 40) | ((uint64)data[82] << 48) | ((uint64)data[83] << 56));
        (void) memcpy((void*)&block->minY, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[84] | ((uint64)data[85] << 8) | ((uint64)data[86] << 16) | ((uint64)data[87] << 24) | 
            ((uint64)data[88] << 32) | ((uint64)data[89] << 40) | ((uint64)data[90] << 48) | ((uint64)data[91] << 56));
        (void) memcpy((void*)&block->maxY, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[92] | ((uint64)data[93] << 8) | ((uint64)data[94] << 16) | ((uint64)data[95] << 24) | 
            ((uint64)data[96] << 32) | ((uint64)data[97] << 40) | ((uint64)data[98] << 48) | ((uint64)data[99] << 56));
        (void) memcpy((void*)&block->minZ, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[100] | ((uint64)data[101] << 8) | ((uint64)data[102] << 16) | ((uint64)data[103] << 24) | 
            ((uint64)data[104] << 32) | ((uint64)data[105] << 40) | ((uint64)data[106] << 48) | ((uint64)data[107] << 56));
        (void) memcpy((void*)&block->maxZ, (const void*)&tmp64, sizeof(float64));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FLOAT64_ISLINKED)
void* LookupTable3D_Float64_GetAddress(LOOKUPTABLE3D_FLOAT64* block, uint16 elementId)
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
