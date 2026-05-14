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
/* Description: Three dimensional look-up table with selectable number of entries and trilinear interpolation */
/* USERCODE-END:Description                                                                                           */
#include "LookupTable3D_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable3D_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X        (*pTLookupTable3D_Float64->x)
#define IN_Y        (*pTLookupTable3D_Float64->y)
#define IN_Z        (*pTLookupTable3D_Float64->z)

/* Outputs */
#define OUT         (pTLookupTable3D_Float64->Out)

/* Parameters */
#define TABLE_DATA  (pTLookupTable3D_Float64->Table)
#define SIZE_X      (pTLookupTable3D_Float64->sizeX)
#define SIZE_Y      (pTLookupTable3D_Float64->sizeY)
#define SIZE_XY     (pTLookupTable3D_Float64->sizeXY)

#define DIM_X       (pTLookupTable3D_Float64->dimX)
#define GAIN_X      (pTLookupTable3D_Float64->gainX)
#define OFFSET_X    (pTLookupTable3D_Float64->offsetX)

#define DIM_Y       (pTLookupTable3D_Float64->dimY)
#define GAIN_Y      (pTLookupTable3D_Float64->gainY)
#define OFFSET_Y    (pTLookupTable3D_Float64->offsetY)

#define DIM_Z       (pTLookupTable3D_Float64->dimZ)
#define GAIN_Z      (pTLookupTable3D_Float64->gainZ)
#define OFFSET_Z    (pTLookupTable3D_Float64->offsetZ)


/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable3D_Float64_Update(LOOKUPTABLE3D_FLOAT64 *pTLookupTable3D_Float64)
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
void LookupTable3D_Float64_Init(LOOKUPTABLE3D_FLOAT64 *pTLookupTable3D_Float64)
{
    pTLookupTable3D_Float64->ID = LOOKUPTABLE3D_FLOAT64_ID;
    pTLookupTable3D_Float64->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float64_Load(const LOOKUPTABLE3D_FLOAT64 *pTLookupTable3D_Float64, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)108 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTLookupTable3D_Float64->dimX & 0x00FF);
        data[1] = (uint8)((pTLookupTable3D_Float64->dimX >> 8) & 0x00FF);
        data[2] = (uint8)(pTLookupTable3D_Float64->dimY & 0x00FF);
        data[3] = (uint8)((pTLookupTable3D_Float64->dimY >> 8) & 0x00FF);
        data[4] = (uint8)(pTLookupTable3D_Float64->dimZ & 0x00FF);
        data[5] = (uint8)((pTLookupTable3D_Float64->dimZ >> 8) & 0x00FF);
        data[6] = (uint8)(pTLookupTable3D_Float64->sizeX & 0x00FF);
        data[7] = (uint8)((pTLookupTable3D_Float64->sizeX >> 8) & 0x00FF);
        data[8] = (uint8)(pTLookupTable3D_Float64->sizeY & 0x00FF);
        data[9] = (uint8)((pTLookupTable3D_Float64->sizeY >> 8) & 0x00FF);
        data[10] = (uint8)(pTLookupTable3D_Float64->sizeXY & 0x00FF);
        data[11] = (uint8)((pTLookupTable3D_Float64->sizeXY >> 8) & 0x00FF);
        data[12] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX)) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 8) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 16) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 24) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 32) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 40) & 0x00000000000000FF);
        data[18] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 48) & 0x00000000000000FF);
        data[19] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainX) >> 56) & 0x00000000000000FF);
        data[20] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY)) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 8) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 16) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 24) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 32) & 0x00000000000000FF);
        data[25] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 40) & 0x00000000000000FF);
        data[26] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 48) & 0x00000000000000FF);
        data[27] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainY) >> 56) & 0x00000000000000FF);
        data[28] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ)) & 0x00000000000000FF);
        data[29] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 8) & 0x00000000000000FF);
        data[30] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 16) & 0x00000000000000FF);
        data[31] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 24) & 0x00000000000000FF);
        data[32] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 32) & 0x00000000000000FF);
        data[33] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 40) & 0x00000000000000FF);
        data[34] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 48) & 0x00000000000000FF);
        data[35] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->gainZ) >> 56) & 0x00000000000000FF);
        data[36] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX)) & 0x00000000000000FF);
        data[37] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 8) & 0x00000000000000FF);
        data[38] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 16) & 0x00000000000000FF);
        data[39] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 24) & 0x00000000000000FF);
        data[40] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 32) & 0x00000000000000FF);
        data[41] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 40) & 0x00000000000000FF);
        data[42] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 48) & 0x00000000000000FF);
        data[43] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetX) >> 56) & 0x00000000000000FF);
        data[44] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY)) & 0x00000000000000FF);
        data[45] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 8) & 0x00000000000000FF);
        data[46] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 16) & 0x00000000000000FF);
        data[47] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 24) & 0x00000000000000FF);
        data[48] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 32) & 0x00000000000000FF);
        data[49] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 40) & 0x00000000000000FF);
        data[50] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 48) & 0x00000000000000FF);
        data[51] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetY) >> 56) & 0x00000000000000FF);
        data[52] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ)) & 0x00000000000000FF);
        data[53] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 8) & 0x00000000000000FF);
        data[54] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 16) & 0x00000000000000FF);
        data[55] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 24) & 0x00000000000000FF);
        data[56] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 32) & 0x00000000000000FF);
        data[57] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 40) & 0x00000000000000FF);
        data[58] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 48) & 0x00000000000000FF);
        data[59] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->offsetZ) >> 56) & 0x00000000000000FF);
        data[60] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX)) & 0x00000000000000FF);
        data[61] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 8) & 0x00000000000000FF);
        data[62] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 16) & 0x00000000000000FF);
        data[63] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 24) & 0x00000000000000FF);
        data[64] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 32) & 0x00000000000000FF);
        data[65] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 40) & 0x00000000000000FF);
        data[66] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 48) & 0x00000000000000FF);
        data[67] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minX) >> 56) & 0x00000000000000FF);
        data[68] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX)) & 0x00000000000000FF);
        data[69] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 8) & 0x00000000000000FF);
        data[70] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 16) & 0x00000000000000FF);
        data[71] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 24) & 0x00000000000000FF);
        data[72] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 32) & 0x00000000000000FF);
        data[73] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 40) & 0x00000000000000FF);
        data[74] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 48) & 0x00000000000000FF);
        data[75] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxX) >> 56) & 0x00000000000000FF);
        data[76] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY)) & 0x00000000000000FF);
        data[77] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 8) & 0x00000000000000FF);
        data[78] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 16) & 0x00000000000000FF);
        data[79] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 24) & 0x00000000000000FF);
        data[80] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 32) & 0x00000000000000FF);
        data[81] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 40) & 0x00000000000000FF);
        data[82] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 48) & 0x00000000000000FF);
        data[83] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minY) >> 56) & 0x00000000000000FF);
        data[84] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY)) & 0x00000000000000FF);
        data[85] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 8) & 0x00000000000000FF);
        data[86] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 16) & 0x00000000000000FF);
        data[87] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 24) & 0x00000000000000FF);
        data[88] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 32) & 0x00000000000000FF);
        data[89] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 40) & 0x00000000000000FF);
        data[90] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 48) & 0x00000000000000FF);
        data[91] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxY) >> 56) & 0x00000000000000FF);
        data[92] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ)) & 0x00000000000000FF);
        data[93] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 8) & 0x00000000000000FF);
        data[94] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 16) & 0x00000000000000FF);
        data[95] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 24) & 0x00000000000000FF);
        data[96] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 32) & 0x00000000000000FF);
        data[97] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 40) & 0x00000000000000FF);
        data[98] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 48) & 0x00000000000000FF);
        data[99] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->minZ) >> 56) & 0x00000000000000FF);
        data[100] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ)) & 0x00000000000000FF);
        data[101] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 8) & 0x00000000000000FF);
        data[102] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 16) & 0x00000000000000FF);
        data[103] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 24) & 0x00000000000000FF);
        data[104] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 32) & 0x00000000000000FF);
        data[105] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 40) & 0x00000000000000FF);
        data[106] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 48) & 0x00000000000000FF);
        data[107] = (uint8)((*(uint64*)&(pTLookupTable3D_Float64->maxZ) >> 56) & 0x00000000000000FF);
        *dataLength = (uint16)108;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable3D_Float64_Save(LOOKUPTABLE3D_FLOAT64 *pTLookupTable3D_Float64, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    uint64 tmp64;

    if (dataLength != (uint16)108)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable3D_Float64->dimX = ((uint16)data[0] + \
            ((uint16)data[1] << 8));
        pTLookupTable3D_Float64->dimY = ((uint16)data[2] + \
            ((uint16)data[3] << 8));
        pTLookupTable3D_Float64->dimZ = ((uint16)data[4] + \
            ((uint16)data[5] << 8));
        pTLookupTable3D_Float64->sizeX = ((uint16)data[6] + \
            ((uint16)data[7] << 8));
        pTLookupTable3D_Float64->sizeY = ((uint16)data[8] + \
            ((uint16)data[9] << 8));
        pTLookupTable3D_Float64->sizeXY = ((uint16)data[10] + \
            ((uint16)data[11] << 8));
        tmp64 = (uint64)data[12] + \
            ((uint64)data[13] << 8) + ((uint64)data[14] << 16) + \
            ((uint64)data[15] << 24) + ((uint64)data[16] << 32) + \
            ((uint64)data[17] << 40) + ((uint64)data[18] << 48) + \
            ((uint64)data[19] << 56);
        pTLookupTable3D_Float64->gainX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[20] + \
            ((uint64)data[21] << 8) + ((uint64)data[22] << 16) + \
            ((uint64)data[23] << 24) + ((uint64)data[24] << 32) + \
            ((uint64)data[25] << 40) + ((uint64)data[26] << 48) + \
            ((uint64)data[27] << 56);
        pTLookupTable3D_Float64->gainY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[28] + \
            ((uint64)data[29] << 8) + ((uint64)data[30] << 16) + \
            ((uint64)data[31] << 24) + ((uint64)data[32] << 32) + \
            ((uint64)data[33] << 40) + ((uint64)data[34] << 48) + \
            ((uint64)data[35] << 56);
        pTLookupTable3D_Float64->gainZ = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[36] + \
            ((uint64)data[37] << 8) + ((uint64)data[38] << 16) + \
            ((uint64)data[39] << 24) + ((uint64)data[40] << 32) + \
            ((uint64)data[41] << 40) + ((uint64)data[42] << 48) + \
            ((uint64)data[43] << 56);
        pTLookupTable3D_Float64->offsetX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[44] + \
            ((uint64)data[45] << 8) + ((uint64)data[46] << 16) + \
            ((uint64)data[47] << 24) + ((uint64)data[48] << 32) + \
            ((uint64)data[49] << 40) + ((uint64)data[50] << 48) + \
            ((uint64)data[51] << 56);
        pTLookupTable3D_Float64->offsetY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[52] + \
            ((uint64)data[53] << 8) + ((uint64)data[54] << 16) + \
            ((uint64)data[55] << 24) + ((uint64)data[56] << 32) + \
            ((uint64)data[57] << 40) + ((uint64)data[58] << 48) + \
            ((uint64)data[59] << 56);
        pTLookupTable3D_Float64->offsetZ = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[60] + \
            ((uint64)data[61] << 8) + ((uint64)data[62] << 16) + \
            ((uint64)data[63] << 24) + ((uint64)data[64] << 32) + \
            ((uint64)data[65] << 40) + ((uint64)data[66] << 48) + \
            ((uint64)data[67] << 56);
        pTLookupTable3D_Float64->minX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[68] + \
            ((uint64)data[69] << 8) + ((uint64)data[70] << 16) + \
            ((uint64)data[71] << 24) + ((uint64)data[72] << 32) + \
            ((uint64)data[73] << 40) + ((uint64)data[74] << 48) + \
            ((uint64)data[75] << 56);
        pTLookupTable3D_Float64->maxX = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[76] + \
            ((uint64)data[77] << 8) + ((uint64)data[78] << 16) + \
            ((uint64)data[79] << 24) + ((uint64)data[80] << 32) + \
            ((uint64)data[81] << 40) + ((uint64)data[82] << 48) + \
            ((uint64)data[83] << 56);
        pTLookupTable3D_Float64->minY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[84] + \
            ((uint64)data[85] << 8) + ((uint64)data[86] << 16) + \
            ((uint64)data[87] << 24) + ((uint64)data[88] << 32) + \
            ((uint64)data[89] << 40) + ((uint64)data[90] << 48) + \
            ((uint64)data[91] << 56);
        pTLookupTable3D_Float64->maxY = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[92] + \
            ((uint64)data[93] << 8) + ((uint64)data[94] << 16) + \
            ((uint64)data[95] << 24) + ((uint64)data[96] << 32) + \
            ((uint64)data[97] << 40) + ((uint64)data[98] << 48) + \
            ((uint64)data[99] << 56);
        pTLookupTable3D_Float64->minZ = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[100] + \
            ((uint64)data[101] << 8) + ((uint64)data[102] << 16) + \
            ((uint64)data[103] << 24) + ((uint64)data[104] << 32) + \
            ((uint64)data[105] << 40) + ((uint64)data[106] << 48) + \
            ((uint64)data[107] << 56);
        pTLookupTable3D_Float64->maxZ = (float64)(*(float64*)&tmp64);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE3D_FLOAT64_ISLINKED)
void* LookupTable3D_Float64_GetAddress(const LOOKUPTABLE3D_FLOAT64* block, uint16 elementId)
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
