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
#include "LookupTable3D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*block->x)
#define IN_Y            (*block->y)
#define IN_Z            (*block->z)

/* Outputs */
#define OUT             (block->Out)

/* Parameters */
#define TABLE_DATA      (block->Table)
#define SIZE_X          (block->sizeX)
#define SIZE_XY         (block->sizeXY)

#define SFR_X           (block->sfrX)
#define MASK_X          (block->maskX)
#define IDX_OFFSET_X    (block->idxOffsetX)
#define MIN_X           (block->minX)
#define MAX_X           (block->maxX)
#define GAIN_X          (block->gainX)
#define GAIN_X_SFR      (block->gainXsfr)
#define OFFSET_X        (block->offsetX)

#define SFR_Y           (block->sfrY)
#define MASK_Y          (block->maskY)
#define IDX_OFFSET_Y    (block->idxOffsetY)
#define MIN_Y           (block->minY)
#define MAX_Y           (block->maxY)
#define GAIN_Y          (block->gainY)
#define GAIN_Y_SFR      (block->gainYsfr)
#define OFFSET_Y        (block->offsetY)

#define SFR_Z           (block->sfrZ)
#define MASK_Z          (block->maskZ)
#define IDX_OFFSET_Z    (block->idxOffsetZ)
#define MIN_Z           (block->minZ)
#define MAX_Z           (block->maxZ)
#define GAIN_Z          (block->gainZ)
#define GAIN_Z_SFR      (block->gainZsfr)
#define OFFSET_Z        (block->offsetZ)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_FiP16_Update(LOOKUPTABLE3D_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 tmp;
    uint16 idx, idxx, idxy, idxz, idx1;
    int16 dx, dy, dz;
    int16 f000, f001, f010, f011, f100, f101, f110, f111;
    int32 c0, c1, c2, c3, c4, c5, c6, c7;
    int16 inx, iny, inz;

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
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        inx = ((int32)tmp * (int32)GAIN_X) >> GAIN_X_SFR;
    }
    if (IN_Y < MIN_Y)
    {
        /* input below lower boundary -> cutoff */
        iny = -INT16_MAX;
    }
    else if (IN_Y > MAX_Y)
    {
        /* input above higher boundary -> cutoff */
        iny = INT16_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int32)IN_Y - (int32)OFFSET_Y);
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        iny = ((int32)tmp * (int32)GAIN_Y) >> GAIN_Y_SFR;
    }
    if (IN_Z < MIN_Z)
    {
        /* input below lower boundary -> cutoff */
        inz = -INT16_MAX;
    }
    else if (IN_Z > MAX_Z)
    {
        /* input above higher boundary -> cutoff */
        inz = INT16_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int32)IN_Z - (int32)OFFSET_Z);
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        inz = ((int32)tmp * (int32)GAIN_Z) >> GAIN_Z_SFR;
    }

    /** - calculate table indexes */
    /* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    idxx = (inx >> SFR_X) + IDX_OFFSET_X;  /*  x  */
    idxy = (iny >> SFR_Y) + IDX_OFFSET_Y;  /*  y  */
    idxz = (inz >> SFR_Z) + IDX_OFFSET_Z;  /*  z  */

    /** - calculate fractual part */
    dx = inx & MASK_X;                     /* dx  */
    dy = iny & MASK_Y;                     /* dy  */
    dz = inz & MASK_Z;                     /* dz  */

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
    c4 = f110 - f010 - c1;                              /* c4 = f110 - f010 - f100 + f000 */
    c5 = f011 - f001 - c2;                              /* c5 = f011 - f001 - f010 + f000 */
    c6 = f101 - f001 - c1;                              /* c6 = f101 - f001 - f100 + f000 */
    c7 = f111 - f011 - f101 - f110 + f100 + f001 + c2;  /* c7 = f111 - f011 - f101 - f110 + f100 + f001 + f010 - f000 */

    /** - trilinear interpolation */
    tmp  = c0;
    /* RULECHECKER_comment(1:0, 7:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    tmp += (((int32)c1 * (int32)dx) >> SFR_X);
    tmp += (((int32)c2 * (int32)dy) >> SFR_Y);
    tmp += (((int32)c3 * (int32)dz) >> SFR_Z);
    tmp += (((((int32)c4 * (int32)dx) >> SFR_X) * (int32)dy) >> SFR_Y);
    tmp += (((((int32)c5 * (int32)dy) >> SFR_Y) * (int32)dz) >> SFR_Z);
    tmp += (((((int32)c6 * (int32)dx) >> SFR_X) * (int32)dz) >> SFR_Z);
    tmp += (((((((int32)c7 * (int32)dx) >> SFR_X) * (int32)dy) >> SFR_Y) * (int32)dz) >> SFR_Z);

    OUT = (int16)tmp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable3D_FiP16_Init(LOOKUPTABLE3D_FIP16 *block)
{
    block->ID = LOOKUPTABLE3D_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP16_Load(const LOOKUPTABLE3D_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 48U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->sfrX & 0xFFU);
        data[1] = (uint8)(block->sfrY & 0xFFU);
        data[2] = (uint8)(block->sfrZ & 0xFFU);
        data[3] = (uint8)(block->maskX & 0xFFU);
        data[4] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[5] = (uint8)(block->maskY & 0xFFU);
        data[6] = (uint8)((block->maskY >> 8) & 0xFFU);
        data[7] = (uint8)(block->maskZ & 0xFFU);
        data[8] = (uint8)((block->maskZ >> 8) & 0xFFU);
        data[9] = (uint8)(block->idxOffsetX & 0xFFU);
        data[10] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[11] = (uint8)(block->idxOffsetY & 0xFFU);
        data[12] = (uint8)((block->idxOffsetY >> 8) & 0xFFU);
        data[13] = (uint8)(block->idxOffsetZ & 0xFFU);
        data[14] = (uint8)((block->idxOffsetZ >> 8) & 0xFFU);
        data[15] = (uint8)(block->sizeX & 0xFFU);
        data[16] = (uint8)((block->sizeX >> 8) & 0xFFU);
        data[17] = (uint8)(block->sizeY & 0xFFU);
        data[18] = (uint8)((block->sizeY >> 8) & 0xFFU);
        data[19] = (uint8)(block->sizeXY & 0xFFU);
        data[20] = (uint8)((block->sizeXY >> 8) & 0xFFU);
        data[21] = (uint8)((uint16)block->gainX & 0xFFU);
        data[22] = (uint8)(((uint16)block->gainX >> 8) & 0xFFU);
        data[23] = (uint8)((uint16)block->gainY & 0xFFU);
        data[24] = (uint8)(((uint16)block->gainY >> 8) & 0xFFU);
        data[25] = (uint8)((uint16)block->gainZ & 0xFFU);
        data[26] = (uint8)(((uint16)block->gainZ >> 8) & 0xFFU);
        data[27] = (uint8)(block->gainXsfr & 0xFFU);
        data[28] = (uint8)(block->gainYsfr & 0xFFU);
        data[29] = (uint8)(block->gainZsfr & 0xFFU);
        data[30] = (uint8)((uint16)block->offsetX & 0xFFU);
        data[31] = (uint8)(((uint16)block->offsetX >> 8) & 0xFFU);
        data[32] = (uint8)((uint16)block->offsetY & 0xFFU);
        data[33] = (uint8)(((uint16)block->offsetY >> 8) & 0xFFU);
        data[34] = (uint8)((uint16)block->offsetZ & 0xFFU);
        data[35] = (uint8)(((uint16)block->offsetZ >> 8) & 0xFFU);
        data[36] = (uint8)((uint16)block->minX & 0xFFU);
        data[37] = (uint8)(((uint16)block->minX >> 8) & 0xFFU);
        data[38] = (uint8)((uint16)block->maxX & 0xFFU);
        data[39] = (uint8)(((uint16)block->maxX >> 8) & 0xFFU);
        data[40] = (uint8)((uint16)block->minY & 0xFFU);
        data[41] = (uint8)(((uint16)block->minY >> 8) & 0xFFU);
        data[42] = (uint8)((uint16)block->maxY & 0xFFU);
        data[43] = (uint8)(((uint16)block->maxY >> 8) & 0xFFU);
        data[44] = (uint8)((uint16)block->minZ & 0xFFU);
        data[45] = (uint8)(((uint16)block->minZ >> 8) & 0xFFU);
        data[46] = (uint8)((uint16)block->maxZ & 0xFFU);
        data[47] = (uint8)(((uint16)block->maxZ >> 8) & 0xFFU);
        *dataLength = 48U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP16_Save(LOOKUPTABLE3D_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 48U)
    {
        error = 1U;
    }
    else
    {
        block->sfrX = data[0];
        block->sfrY = data[1];
        block->sfrZ = data[2];
        block->maskX = (uint16)((uint16)data[3] | ((uint16)data[4] << 8));
        block->maskY = (uint16)((uint16)data[5] | ((uint16)data[6] << 8));
        block->maskZ = (uint16)((uint16)data[7] | ((uint16)data[8] << 8));
        block->idxOffsetX = (uint16)((uint16)data[9] | ((uint16)data[10] << 8));
        block->idxOffsetY = (uint16)((uint16)data[11] | ((uint16)data[12] << 8));
        block->idxOffsetZ = (uint16)((uint16)data[13] | ((uint16)data[14] << 8));
        block->sizeX = (uint16)((uint16)data[15] | ((uint16)data[16] << 8));
        block->sizeY = (uint16)((uint16)data[17] | ((uint16)data[18] << 8));
        block->sizeXY = (uint16)((uint16)data[19] | ((uint16)data[20] << 8));
        block->gainX = castUint16ToInt16((uint16)((uint16)data[21] | ((uint16)data[22] << 8)));
        block->gainY = castUint16ToInt16((uint16)((uint16)data[23] | ((uint16)data[24] << 8)));
        block->gainZ = castUint16ToInt16((uint16)((uint16)data[25] | ((uint16)data[26] << 8)));
        block->gainXsfr = data[27];
        block->gainYsfr = data[28];
        block->gainZsfr = data[29];
        block->offsetX = castUint16ToInt16((uint16)((uint16)data[30] | ((uint16)data[31] << 8)));
        block->offsetY = castUint16ToInt16((uint16)((uint16)data[32] | ((uint16)data[33] << 8)));
        block->offsetZ = castUint16ToInt16((uint16)((uint16)data[34] | ((uint16)data[35] << 8)));
        block->minX = castUint16ToInt16((uint16)((uint16)data[36] | ((uint16)data[37] << 8)));
        block->maxX = castUint16ToInt16((uint16)((uint16)data[38] | ((uint16)data[39] << 8)));
        block->minY = castUint16ToInt16((uint16)((uint16)data[40] | ((uint16)data[41] << 8)));
        block->maxY = castUint16ToInt16((uint16)((uint16)data[42] | ((uint16)data[43] << 8)));
        block->minZ = castUint16ToInt16((uint16)((uint16)data[44] | ((uint16)data[45] << 8)));
        block->maxZ = castUint16ToInt16((uint16)((uint16)data[46] | ((uint16)data[47] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FIP16_ISLINKED)
void* LookupTable3D_FiP16_GetAddress(LOOKUPTABLE3D_FIP16* block, uint16 elementId)
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
