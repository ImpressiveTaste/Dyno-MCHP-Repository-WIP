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
#include "LookupTable3D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*pTLookupTable3D_FiP16->x)
#define IN_Y            (*pTLookupTable3D_FiP16->y)
#define IN_Z            (*pTLookupTable3D_FiP16->z)

/* Outputs */
#define OUT             (pTLookupTable3D_FiP16->Out)

/* Parameters */
#define TABLE_DATA      (pTLookupTable3D_FiP16->Table)
#define SIZE_X          (pTLookupTable3D_FiP16->sizeX)
#define SIZE_Y          (pTLookupTable3D_FiP16->sizeY)
#define SIZE_XY         (pTLookupTable3D_FiP16->sizeXY)

#define SFR_X           (pTLookupTable3D_FiP16->sfrX)
#define MASK_X          (pTLookupTable3D_FiP16->maskX)
#define IDX_OFFSET_X    (pTLookupTable3D_FiP16->idxOffsetX)
#define MIN_X           (pTLookupTable3D_FiP16->minX)
#define MAX_X           (pTLookupTable3D_FiP16->maxX)
#define GAIN_X          (pTLookupTable3D_FiP16->gainX)
#define GAIN_X_SFR      (pTLookupTable3D_FiP16->gainXsfr)
#define OFFSET_X        (pTLookupTable3D_FiP16->offsetX)

#define SFR_Y           (pTLookupTable3D_FiP16->sfrY)
#define MASK_Y          (pTLookupTable3D_FiP16->maskY)
#define IDX_OFFSET_Y    (pTLookupTable3D_FiP16->idxOffsetY)
#define MIN_Y           (pTLookupTable3D_FiP16->minY)
#define MAX_Y           (pTLookupTable3D_FiP16->maxY)
#define GAIN_Y          (pTLookupTable3D_FiP16->gainY)
#define GAIN_Y_SFR      (pTLookupTable3D_FiP16->gainYsfr)
#define OFFSET_Y        (pTLookupTable3D_FiP16->offsetY)

#define SFR_Z           (pTLookupTable3D_FiP16->sfrZ)
#define MASK_Z          (pTLookupTable3D_FiP16->maskZ)
#define IDX_OFFSET_Z    (pTLookupTable3D_FiP16->idxOffsetZ)
#define MIN_Z           (pTLookupTable3D_FiP16->minZ)
#define MAX_Z           (pTLookupTable3D_FiP16->maxZ)
#define GAIN_Z          (pTLookupTable3D_FiP16->gainZ)
#define GAIN_Z_SFR      (pTLookupTable3D_FiP16->gainZsfr)
#define OFFSET_Z        (pTLookupTable3D_FiP16->offsetZ)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_FiP16_Update(LOOKUPTABLE3D_FIP16 *pTLookupTable3D_FiP16)
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
        inz = ((int32)tmp * (int32)GAIN_Z) >> GAIN_Z_SFR;
    }
 
    /** - calculate table indexes */
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
void LookupTable3D_FiP16_Init(LOOKUPTABLE3D_FIP16 *pTLookupTable3D_FiP16)
{
    pTLookupTable3D_FiP16->ID = LOOKUPTABLE3D_FIP16_ID;
    pTLookupTable3D_FiP16->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP16_Load(const LOOKUPTABLE3D_FIP16 *pTLookupTable3D_FiP16, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)48 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)pTLookupTable3D_FiP16->sfrX;
        data[1] = (uint8)pTLookupTable3D_FiP16->sfrY;
        data[2] = (uint8)pTLookupTable3D_FiP16->sfrZ;
        data[3] = (uint8)(pTLookupTable3D_FiP16->maskX & 0x00FF);
        data[4] = (uint8)((pTLookupTable3D_FiP16->maskX >> 8) & 0x00FF);
        data[5] = (uint8)(pTLookupTable3D_FiP16->maskY & 0x00FF);
        data[6] = (uint8)((pTLookupTable3D_FiP16->maskY >> 8) & 0x00FF);
        data[7] = (uint8)(pTLookupTable3D_FiP16->maskZ & 0x00FF);
        data[8] = (uint8)((pTLookupTable3D_FiP16->maskZ >> 8) & 0x00FF);
        data[9] = (uint8)(pTLookupTable3D_FiP16->idxOffsetX & 0x00FF);
        data[10] = (uint8)((pTLookupTable3D_FiP16->idxOffsetX >> 8) & 0x00FF);
        data[11] = (uint8)(pTLookupTable3D_FiP16->idxOffsetY & 0x00FF);
        data[12] = (uint8)((pTLookupTable3D_FiP16->idxOffsetY >> 8) & 0x00FF);
        data[13] = (uint8)(pTLookupTable3D_FiP16->idxOffsetZ & 0x00FF);
        data[14] = (uint8)((pTLookupTable3D_FiP16->idxOffsetZ >> 8) & 0x00FF);
        data[15] = (uint8)(pTLookupTable3D_FiP16->sizeX & 0x00FF);
        data[16] = (uint8)((pTLookupTable3D_FiP16->sizeX >> 8) & 0x00FF);
        data[17] = (uint8)(pTLookupTable3D_FiP16->sizeY & 0x00FF);
        data[18] = (uint8)((pTLookupTable3D_FiP16->sizeY >> 8) & 0x00FF);
        data[19] = (uint8)(pTLookupTable3D_FiP16->sizeXY & 0x00FF);
        data[20] = (uint8)((pTLookupTable3D_FiP16->sizeXY >> 8) & 0x00FF);
        data[21] = (uint8)(pTLookupTable3D_FiP16->gainX & 0x00FF);
        data[22] = (uint8)((pTLookupTable3D_FiP16->gainX >> 8) & 0x00FF);
        data[23] = (uint8)(pTLookupTable3D_FiP16->gainY & 0x00FF);
        data[24] = (uint8)((pTLookupTable3D_FiP16->gainY >> 8) & 0x00FF);
        data[25] = (uint8)(pTLookupTable3D_FiP16->gainZ & 0x00FF);
        data[26] = (uint8)((pTLookupTable3D_FiP16->gainZ >> 8) & 0x00FF);
        data[27] = (uint8)pTLookupTable3D_FiP16->gainXsfr;
        data[28] = (uint8)pTLookupTable3D_FiP16->gainYsfr;
        data[29] = (uint8)pTLookupTable3D_FiP16->gainZsfr;
        data[30] = (uint8)(pTLookupTable3D_FiP16->offsetX & 0x00FF);
        data[31] = (uint8)((pTLookupTable3D_FiP16->offsetX >> 8) & 0x00FF);
        data[32] = (uint8)(pTLookupTable3D_FiP16->offsetY & 0x00FF);
        data[33] = (uint8)((pTLookupTable3D_FiP16->offsetY >> 8) & 0x00FF);
        data[34] = (uint8)(pTLookupTable3D_FiP16->offsetZ & 0x00FF);
        data[35] = (uint8)((pTLookupTable3D_FiP16->offsetZ >> 8) & 0x00FF);
        data[36] = (uint8)(pTLookupTable3D_FiP16->minX & 0x00FF);
        data[37] = (uint8)((pTLookupTable3D_FiP16->minX >> 8) & 0x00FF);
        data[38] = (uint8)(pTLookupTable3D_FiP16->maxX & 0x00FF);
        data[39] = (uint8)((pTLookupTable3D_FiP16->maxX >> 8) & 0x00FF);
        data[40] = (uint8)(pTLookupTable3D_FiP16->minY & 0x00FF);
        data[41] = (uint8)((pTLookupTable3D_FiP16->minY >> 8) & 0x00FF);
        data[42] = (uint8)(pTLookupTable3D_FiP16->maxY & 0x00FF);
        data[43] = (uint8)((pTLookupTable3D_FiP16->maxY >> 8) & 0x00FF);
        data[44] = (uint8)(pTLookupTable3D_FiP16->minZ & 0x00FF);
        data[45] = (uint8)((pTLookupTable3D_FiP16->minZ >> 8) & 0x00FF);
        data[46] = (uint8)(pTLookupTable3D_FiP16->maxZ & 0x00FF);
        data[47] = (uint8)((pTLookupTable3D_FiP16->maxZ >> 8) & 0x00FF);
        *dataLength = (uint16)48;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_FiP16_Save(LOOKUPTABLE3D_FIP16 *pTLookupTable3D_FiP16, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)48)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable3D_FiP16->sfrX = UINT8_TO_INT8((uint8)data[0]);
        pTLookupTable3D_FiP16->sfrY = UINT8_TO_INT8((uint8)data[1]);
        pTLookupTable3D_FiP16->sfrZ = UINT8_TO_INT8((uint8)data[2]);
        pTLookupTable3D_FiP16->maskX = ((uint16)data[3] + \
            ((uint16)data[4] << 8));
        pTLookupTable3D_FiP16->maskY = ((uint16)data[5] + \
            ((uint16)data[6] << 8));
        pTLookupTable3D_FiP16->maskZ = ((uint16)data[7] + \
            ((uint16)data[8] << 8));
        pTLookupTable3D_FiP16->idxOffsetX = ((uint16)data[9] + \
            ((uint16)data[10] << 8));
        pTLookupTable3D_FiP16->idxOffsetY = ((uint16)data[11] + \
            ((uint16)data[12] << 8));
        pTLookupTable3D_FiP16->idxOffsetZ = ((uint16)data[13] + \
            ((uint16)data[14] << 8));
        pTLookupTable3D_FiP16->sizeX = ((uint16)data[15] + \
            ((uint16)data[16] << 8));
        pTLookupTable3D_FiP16->sizeY = ((uint16)data[17] + \
            ((uint16)data[18] << 8));
        pTLookupTable3D_FiP16->sizeXY = ((uint16)data[19] + \
            ((uint16)data[20] << 8));
        pTLookupTable3D_FiP16->gainX = UINT16_TO_INT16((uint16)data[21] + \
            ((uint16)data[22] << 8));
        pTLookupTable3D_FiP16->gainY = UINT16_TO_INT16((uint16)data[23] + \
            ((uint16)data[24] << 8));
        pTLookupTable3D_FiP16->gainZ = UINT16_TO_INT16((uint16)data[25] + \
            ((uint16)data[26] << 8));
        pTLookupTable3D_FiP16->gainXsfr = UINT8_TO_INT8((uint8)data[27]);
        pTLookupTable3D_FiP16->gainYsfr = UINT8_TO_INT8((uint8)data[28]);
        pTLookupTable3D_FiP16->gainZsfr = UINT8_TO_INT8((uint8)data[29]);
        pTLookupTable3D_FiP16->offsetX = UINT16_TO_INT16((uint16)data[30] + \
            ((uint16)data[31] << 8));
        pTLookupTable3D_FiP16->offsetY = UINT16_TO_INT16((uint16)data[32] + \
            ((uint16)data[33] << 8));
        pTLookupTable3D_FiP16->offsetZ = UINT16_TO_INT16((uint16)data[34] + \
            ((uint16)data[35] << 8));
        pTLookupTable3D_FiP16->minX = UINT16_TO_INT16((uint16)data[36] + \
            ((uint16)data[37] << 8));
        pTLookupTable3D_FiP16->maxX = UINT16_TO_INT16((uint16)data[38] + \
            ((uint16)data[39] << 8));
        pTLookupTable3D_FiP16->minY = UINT16_TO_INT16((uint16)data[40] + \
            ((uint16)data[41] << 8));
        pTLookupTable3D_FiP16->maxY = UINT16_TO_INT16((uint16)data[42] + \
            ((uint16)data[43] << 8));
        pTLookupTable3D_FiP16->minZ = UINT16_TO_INT16((uint16)data[44] + \
            ((uint16)data[45] << 8));
        pTLookupTable3D_FiP16->maxZ = UINT16_TO_INT16((uint16)data[46] + \
            ((uint16)data[47] << 8));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FIP16_ISLINKED)
void* LookupTable3D_FiP16_GetAddress(const LOOKUPTABLE3D_FIP16* block, uint16 elementId)
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
