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
/* Description: Three dimensional look-up table with selectable number of entries and trilinear interpolation */
/* USERCODE-END:Description                                                                                           */
#include "LookupTable3D_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*pTLookupTable3D_FiP32->x)
#define IN_Y            (*pTLookupTable3D_FiP32->y)
#define IN_Z            (*pTLookupTable3D_FiP32->z)

/* Outputs */
#define OUT             (pTLookupTable3D_FiP32->Out)

/* Parameters */
#define TABLE_DATA      (pTLookupTable3D_FiP32->Table)
#define SIZE_X          (pTLookupTable3D_FiP32->sizeX)
#define SIZE_Y          (pTLookupTable3D_FiP32->sizeY)
#define SIZE_XY         (pTLookupTable3D_FiP32->sizeXY)

#define SFR_X           (pTLookupTable3D_FiP32->sfrX)
#define MASK_X          (pTLookupTable3D_FiP32->maskX)
#define IDX_OFFSET_X    (pTLookupTable3D_FiP32->idxOffsetX)
#define MIN_X           (pTLookupTable3D_FiP32->minX)
#define MAX_X           (pTLookupTable3D_FiP32->maxX)
#define GAIN_X          (pTLookupTable3D_FiP32->gainX)
#define GAIN_X_SFR      (pTLookupTable3D_FiP32->gainXsfr)
#define OFFSET_X        (pTLookupTable3D_FiP32->offsetX)

#define SFR_Y           (pTLookupTable3D_FiP32->sfrY)
#define MASK_Y          (pTLookupTable3D_FiP32->maskY)
#define IDX_OFFSET_Y    (pTLookupTable3D_FiP32->idxOffsetY)
#define MIN_Y           (pTLookupTable3D_FiP32->minY)
#define MAX_Y           (pTLookupTable3D_FiP32->maxY)
#define GAIN_Y          (pTLookupTable3D_FiP32->gainY)
#define GAIN_Y_SFR      (pTLookupTable3D_FiP32->gainYsfr)
#define OFFSET_Y        (pTLookupTable3D_FiP32->offsetY)

#define SFR_Z           (pTLookupTable3D_FiP32->sfrZ)
#define MASK_Z          (pTLookupTable3D_FiP32->maskZ)
#define IDX_OFFSET_Z    (pTLookupTable3D_FiP32->idxOffsetZ)
#define MIN_Z           (pTLookupTable3D_FiP32->minZ)
#define MAX_Z           (pTLookupTable3D_FiP32->maxZ)
#define GAIN_Z          (pTLookupTable3D_FiP32->gainZ)
#define GAIN_Z_SFR      (pTLookupTable3D_FiP32->gainZsfr)
#define OFFSET_Z        (pTLookupTable3D_FiP32->offsetZ)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_FiP32_Update(LOOKUPTABLE3D_FIP32 *pTLookupTable3D_FiP32)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 tmp;
    uint16 idx, idxx, idxy, idxz, idx1;
    int32 dx, dy, dz;
    int32 f000, f001, f010, f011, f100, f101, f110, f111;
    int64 c0, c1, c2, c3, c4, c5, c6, c7; 
    int32 inx, iny, inz;
    
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
    if (IN_Z < MIN_Z)
    {
        /* input below lower boundary -> cutoff */
        inz = -INT32_MAX;
    }
    else if (IN_Z > MAX_Z)
    {
        /* input above higher boundary -> cutoff */
        inz = INT32_MAX;
    }
    else
    {
        /* input within boundaries -> input scaling */
        tmp = ((int64)IN_Z - (int64)OFFSET_Z);
        inz = ((int64)tmp * (int64)GAIN_Z) >> GAIN_Z_SFR;
    }
    
    /** - calculate table indexes */
    idxx = (inx >> SFR_X) + IDX_OFFSET_X;   /*  x  */
    idxy = (iny >> SFR_Y) + IDX_OFFSET_Y;   /*  y  */
    idxz = (inz >> SFR_Z) + IDX_OFFSET_Z;   /*  z  */

    /** - calculate fractual part */
    dx = inx & MASK_X;                      /* dx  */
    dy = iny & MASK_Y;                      /* dy  */
    dz = inz & MASK_Z;                      /* dz  */
    
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
    tmp  = c0;
    tmp += (((int64)c1 * (int64)dx) >> SFR_X);
    tmp += (((int64)c2 * (int64)dy) >> SFR_Y);
    tmp += (((int64)c3 * (int64)dz) >> SFR_Z);
    tmp += (((((int64)c4 * (int64)dx) >> SFR_X) * (int64)dy) >> SFR_Y);
    tmp += (((((int64)c5 * (int64)dy) >> SFR_Y) * (int64)dz) >> SFR_Z);
    tmp += (((((int64)c6 * (int64)dx) >> SFR_X) * (int64)dz) >> SFR_Z);
    tmp += (((((((int64)c7 * (int64)dx) >> SFR_X) * (int64)dy) >> SFR_Y) * (int64)dz) >> SFR_Z);
    
    OUT = (int32)tmp;
/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable3D_FiP32_Init(LOOKUPTABLE3D_FIP32 *pTLookupTable3D_FiP32)
{
    pTLookupTable3D_FiP32->ID = LOOKUPTABLE3D_FIP32_ID;
    pTLookupTable3D_FiP32->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP32_Load(const LOOKUPTABLE3D_FIP32 *pTLookupTable3D_FiP32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)78 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)pTLookupTable3D_FiP32->sfrX;
        data[1] = (uint8)pTLookupTable3D_FiP32->sfrY;
        data[2] = (uint8)pTLookupTable3D_FiP32->sfrZ;
        data[3] = (uint8)(pTLookupTable3D_FiP32->maskX & 0x000000FF);
        data[4] = (uint8)((pTLookupTable3D_FiP32->maskX >> 8) & 0x000000FF);
        data[5] = (uint8)((pTLookupTable3D_FiP32->maskX >> 16) & 0x000000FF);
        data[6] = (uint8)((pTLookupTable3D_FiP32->maskX >> 24) & 0x000000FF);
        data[7] = (uint8)(pTLookupTable3D_FiP32->maskY & 0x000000FF);
        data[8] = (uint8)((pTLookupTable3D_FiP32->maskY >> 8) & 0x000000FF);
        data[9] = (uint8)((pTLookupTable3D_FiP32->maskY >> 16) & 0x000000FF);
        data[10] = (uint8)((pTLookupTable3D_FiP32->maskY >> 24) & 0x000000FF);
        data[11] = (uint8)(pTLookupTable3D_FiP32->maskZ & 0x000000FF);
        data[12] = (uint8)((pTLookupTable3D_FiP32->maskZ >> 8) & 0x000000FF);
        data[13] = (uint8)((pTLookupTable3D_FiP32->maskZ >> 16) & 0x000000FF);
        data[14] = (uint8)((pTLookupTable3D_FiP32->maskZ >> 24) & 0x000000FF);
        data[15] = (uint8)(pTLookupTable3D_FiP32->idxOffsetX & 0x00FF);
        data[16] = (uint8)((pTLookupTable3D_FiP32->idxOffsetX >> 8) & 0x00FF);
        data[17] = (uint8)(pTLookupTable3D_FiP32->idxOffsetY & 0x00FF);
        data[18] = (uint8)((pTLookupTable3D_FiP32->idxOffsetY >> 8) & 0x00FF);
        data[19] = (uint8)(pTLookupTable3D_FiP32->idxOffsetZ & 0x00FF);
        data[20] = (uint8)((pTLookupTable3D_FiP32->idxOffsetZ >> 8) & 0x00FF);
        data[21] = (uint8)(pTLookupTable3D_FiP32->sizeX & 0x00FF);
        data[22] = (uint8)((pTLookupTable3D_FiP32->sizeX >> 8) & 0x00FF);
        data[23] = (uint8)(pTLookupTable3D_FiP32->sizeY & 0x00FF);
        data[24] = (uint8)((pTLookupTable3D_FiP32->sizeY >> 8) & 0x00FF);
        data[25] = (uint8)(pTLookupTable3D_FiP32->sizeXY & 0x00FF);
        data[26] = (uint8)((pTLookupTable3D_FiP32->sizeXY >> 8) & 0x00FF);
        data[27] = (uint8)(pTLookupTable3D_FiP32->gainX & 0x000000FF);
        data[28] = (uint8)((pTLookupTable3D_FiP32->gainX >> 8) & 0x000000FF);
        data[29] = (uint8)((pTLookupTable3D_FiP32->gainX >> 16) & 0x000000FF);
        data[30] = (uint8)((pTLookupTable3D_FiP32->gainX >> 24) & 0x000000FF);
        data[31] = (uint8)(pTLookupTable3D_FiP32->gainY & 0x000000FF);
        data[32] = (uint8)((pTLookupTable3D_FiP32->gainY >> 8) & 0x000000FF);
        data[33] = (uint8)((pTLookupTable3D_FiP32->gainY >> 16) & 0x000000FF);
        data[34] = (uint8)((pTLookupTable3D_FiP32->gainY >> 24) & 0x000000FF);
        data[35] = (uint8)(pTLookupTable3D_FiP32->gainZ & 0x000000FF);
        data[36] = (uint8)((pTLookupTable3D_FiP32->gainZ >> 8) & 0x000000FF);
        data[37] = (uint8)((pTLookupTable3D_FiP32->gainZ >> 16) & 0x000000FF);
        data[38] = (uint8)((pTLookupTable3D_FiP32->gainZ >> 24) & 0x000000FF);
        data[39] = (uint8)pTLookupTable3D_FiP32->gainXsfr;
        data[40] = (uint8)pTLookupTable3D_FiP32->gainYsfr;
        data[41] = (uint8)pTLookupTable3D_FiP32->gainZsfr;
        data[42] = (uint8)(pTLookupTable3D_FiP32->offsetX & 0x000000FF);
        data[43] = (uint8)((pTLookupTable3D_FiP32->offsetX >> 8) & 0x000000FF);
        data[44] = (uint8)((pTLookupTable3D_FiP32->offsetX >> 16) & 0x000000FF);
        data[45] = (uint8)((pTLookupTable3D_FiP32->offsetX >> 24) & 0x000000FF);
        data[46] = (uint8)(pTLookupTable3D_FiP32->offsetY & 0x000000FF);
        data[47] = (uint8)((pTLookupTable3D_FiP32->offsetY >> 8) & 0x000000FF);
        data[48] = (uint8)((pTLookupTable3D_FiP32->offsetY >> 16) & 0x000000FF);
        data[49] = (uint8)((pTLookupTable3D_FiP32->offsetY >> 24) & 0x000000FF);
        data[50] = (uint8)(pTLookupTable3D_FiP32->offsetZ & 0x000000FF);
        data[51] = (uint8)((pTLookupTable3D_FiP32->offsetZ >> 8) & 0x000000FF);
        data[52] = (uint8)((pTLookupTable3D_FiP32->offsetZ >> 16) & 0x000000FF);
        data[53] = (uint8)((pTLookupTable3D_FiP32->offsetZ >> 24) & 0x000000FF);
        data[54] = (uint8)(pTLookupTable3D_FiP32->minX & 0x000000FF);
        data[55] = (uint8)((pTLookupTable3D_FiP32->minX >> 8) & 0x000000FF);
        data[56] = (uint8)((pTLookupTable3D_FiP32->minX >> 16) & 0x000000FF);
        data[57] = (uint8)((pTLookupTable3D_FiP32->minX >> 24) & 0x000000FF);
        data[58] = (uint8)(pTLookupTable3D_FiP32->maxX & 0x000000FF);
        data[59] = (uint8)((pTLookupTable3D_FiP32->maxX >> 8) & 0x000000FF);
        data[60] = (uint8)((pTLookupTable3D_FiP32->maxX >> 16) & 0x000000FF);
        data[61] = (uint8)((pTLookupTable3D_FiP32->maxX >> 24) & 0x000000FF);
        data[62] = (uint8)(pTLookupTable3D_FiP32->minY & 0x000000FF);
        data[63] = (uint8)((pTLookupTable3D_FiP32->minY >> 8) & 0x000000FF);
        data[64] = (uint8)((pTLookupTable3D_FiP32->minY >> 16) & 0x000000FF);
        data[65] = (uint8)((pTLookupTable3D_FiP32->minY >> 24) & 0x000000FF);
        data[66] = (uint8)(pTLookupTable3D_FiP32->maxY & 0x000000FF);
        data[67] = (uint8)((pTLookupTable3D_FiP32->maxY >> 8) & 0x000000FF);
        data[68] = (uint8)((pTLookupTable3D_FiP32->maxY >> 16) & 0x000000FF);
        data[69] = (uint8)((pTLookupTable3D_FiP32->maxY >> 24) & 0x000000FF);
        data[70] = (uint8)(pTLookupTable3D_FiP32->minZ & 0x000000FF);
        data[71] = (uint8)((pTLookupTable3D_FiP32->minZ >> 8) & 0x000000FF);
        data[72] = (uint8)((pTLookupTable3D_FiP32->minZ >> 16) & 0x000000FF);
        data[73] = (uint8)((pTLookupTable3D_FiP32->minZ >> 24) & 0x000000FF);
        data[74] = (uint8)(pTLookupTable3D_FiP32->maxZ & 0x000000FF);
        data[75] = (uint8)((pTLookupTable3D_FiP32->maxZ >> 8) & 0x000000FF);
        data[76] = (uint8)((pTLookupTable3D_FiP32->maxZ >> 16) & 0x000000FF);
        data[77] = (uint8)((pTLookupTable3D_FiP32->maxZ >> 24) & 0x000000FF);
        *dataLength = (uint16)78;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP32_Save(LOOKUPTABLE3D_FIP32 *pTLookupTable3D_FiP32, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)78)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable3D_FiP32->sfrX = UINT8_TO_INT8((uint8)data[0]);
        pTLookupTable3D_FiP32->sfrY = UINT8_TO_INT8((uint8)data[1]);
        pTLookupTable3D_FiP32->sfrZ = UINT8_TO_INT8((uint8)data[2]);
        pTLookupTable3D_FiP32->maskX = ((uint32)data[3] + \
            ((uint32)data[4] << 8) + ((uint32)data[5] << 16) + \
            ((uint32)data[6] << 24));
        pTLookupTable3D_FiP32->maskY = ((uint32)data[7] + \
            ((uint32)data[8] << 8) + ((uint32)data[9] << 16) + \
            ((uint32)data[10] << 24));
        pTLookupTable3D_FiP32->maskZ = ((uint32)data[11] + \
            ((uint32)data[12] << 8) + ((uint32)data[13] << 16) + \
            ((uint32)data[14] << 24));
        pTLookupTable3D_FiP32->idxOffsetX = ((uint16)data[15] + \
            ((uint16)data[16] << 8));
        pTLookupTable3D_FiP32->idxOffsetY = ((uint16)data[17] + \
            ((uint16)data[18] << 8));
        pTLookupTable3D_FiP32->idxOffsetZ = ((uint16)data[19] + \
            ((uint16)data[20] << 8));
        pTLookupTable3D_FiP32->sizeX = ((uint16)data[21] + \
            ((uint16)data[22] << 8));
        pTLookupTable3D_FiP32->sizeY = ((uint16)data[23] + \
            ((uint16)data[24] << 8));
        pTLookupTable3D_FiP32->sizeXY = ((uint16)data[25] + \
            ((uint16)data[26] << 8));
        pTLookupTable3D_FiP32->gainX = UINT32_TO_INT32((uint32)data[27] + \
            ((uint32)data[28] << 8) + ((uint32)data[29] << 16) + \
            ((uint32)data[30] << 24));
        pTLookupTable3D_FiP32->gainY = UINT32_TO_INT32((uint32)data[31] + \
            ((uint32)data[32] << 8) + ((uint32)data[33] << 16) + \
            ((uint32)data[34] << 24));
        pTLookupTable3D_FiP32->gainZ = UINT32_TO_INT32((uint32)data[35] + \
            ((uint32)data[36] << 8) + ((uint32)data[37] << 16) + \
            ((uint32)data[38] << 24));
        pTLookupTable3D_FiP32->gainXsfr = UINT8_TO_INT8((uint8)data[39]);
        pTLookupTable3D_FiP32->gainYsfr = UINT8_TO_INT8((uint8)data[40]);
        pTLookupTable3D_FiP32->gainZsfr = UINT8_TO_INT8((uint8)data[41]);
        pTLookupTable3D_FiP32->offsetX = UINT32_TO_INT32((uint32)data[42] + \
            ((uint32)data[43] << 8) + ((uint32)data[44] << 16) + \
            ((uint32)data[45] << 24));
        pTLookupTable3D_FiP32->offsetY = UINT32_TO_INT32((uint32)data[46] + \
            ((uint32)data[47] << 8) + ((uint32)data[48] << 16) + \
            ((uint32)data[49] << 24));
        pTLookupTable3D_FiP32->offsetZ = UINT32_TO_INT32((uint32)data[50] + \
            ((uint32)data[51] << 8) + ((uint32)data[52] << 16) + \
            ((uint32)data[53] << 24));
        pTLookupTable3D_FiP32->minX = UINT32_TO_INT32((uint32)data[54] + \
            ((uint32)data[55] << 8) + ((uint32)data[56] << 16) + \
            ((uint32)data[57] << 24));
        pTLookupTable3D_FiP32->maxX = UINT32_TO_INT32((uint32)data[58] + \
            ((uint32)data[59] << 8) + ((uint32)data[60] << 16) + \
            ((uint32)data[61] << 24));
        pTLookupTable3D_FiP32->minY = UINT32_TO_INT32((uint32)data[62] + \
            ((uint32)data[63] << 8) + ((uint32)data[64] << 16) + \
            ((uint32)data[65] << 24));
        pTLookupTable3D_FiP32->maxY = UINT32_TO_INT32((uint32)data[66] + \
            ((uint32)data[67] << 8) + ((uint32)data[68] << 16) + \
            ((uint32)data[69] << 24));
        pTLookupTable3D_FiP32->minZ = UINT32_TO_INT32((uint32)data[70] + \
            ((uint32)data[71] << 8) + ((uint32)data[72] << 16) + \
            ((uint32)data[73] << 24));
        pTLookupTable3D_FiP32->maxZ = UINT32_TO_INT32((uint32)data[74] + \
            ((uint32)data[75] << 8) + ((uint32)data[76] << 16) + \
            ((uint32)data[77] << 24));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FIP32_ISLINKED)
void* LookupTable3D_FiP32_GetAddress(const LOOKUPTABLE3D_FIP32* block, uint16 elementId)
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
            addr = (void*)block->z;
            break;
        case 4:
            addr = (void*)&block->Out;
            break;
        default:
            addr = (void*)0;
            break;
    }
    return (addr);
}
#endif
