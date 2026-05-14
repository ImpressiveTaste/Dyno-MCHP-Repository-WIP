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
 * $LastChangedRevision: 2078 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: Two dimensional look-up table with selectable number of entries */
/* USERCODE-END:Description                                                                                           */
#include "LookupTable2D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable2D_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*pTLookupTable2D_FiP16->x)
#define IN_Y            (*pTLookupTable2D_FiP16->y)

/* Outputs */
#define OUT             (pTLookupTable2D_FiP16->Out)

/* Parameters */
#define TABLE_DATA      (pTLookupTable2D_FiP16->Table)
#define SIZE_X          (pTLookupTable2D_FiP16->sizeX)

#define SFR_X           (pTLookupTable2D_FiP16->sfrX)
#define MASK_X          (pTLookupTable2D_FiP16->maskX)
#define IDX_OFFSET_X    (pTLookupTable2D_FiP16->idxOffsetX)
#define MIN_X           (pTLookupTable2D_FiP16->minX)
#define MAX_X           (pTLookupTable2D_FiP16->maxX)
#define GAIN_X          (pTLookupTable2D_FiP16->gainX)
#define GAIN_X_SFR      (pTLookupTable2D_FiP16->gainXsfr)
#define OFFSET_X        (pTLookupTable2D_FiP16->offsetX)

#define SFR_Y           (pTLookupTable2D_FiP16->sfrY)
#define MASK_Y          (pTLookupTable2D_FiP16->maskY)
#define IDX_OFFSET_Y    (pTLookupTable2D_FiP16->idxOffsetY)
#define MIN_Y           (pTLookupTable2D_FiP16->minY)
#define MAX_Y           (pTLookupTable2D_FiP16->maxY)
#define GAIN_Y          (pTLookupTable2D_FiP16->gainY)
#define GAIN_Y_SFR      (pTLookupTable2D_FiP16->gainYsfr)
#define OFFSET_Y        (pTLookupTable2D_FiP16->offsetY)



/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable2D_FiP16_Update(LOOKUPTABLE2D_FIP16 *pTLookupTable2D_FiP16)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 tmp;
    uint16 idxx, idxy, idx;
    int16 fact, factx, facty, factxy, deltax, deltay, deltaxy;
    int16 inx, iny;
    
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
    
    /** - calculate table indexes */
    idxx   = (inx >> SFR_X) + IDX_OFFSET_X;    /*  x  */
    idxy   = (iny >> SFR_Y) + IDX_OFFSET_Y;    /*  y  */
    idx    = idxx + (idxy*SIZE_X);
    
    /** - calculate fractual part */
    deltax  = inx & MASK_X;                                     /* dx  */
    deltay  = iny & MASK_Y;                                     /* dy  */
    deltaxy = ((int32)(deltax)*(int32)(deltay)) >> SFR_Y;       /* dxy */

    /** - calculate value at lattice points */
    fact   = TABLE_DATA[idx];                                   /* f(x  ,y)   */
    factx  = TABLE_DATA[idx + 1];                               /* f(x+1,y)   */
    facty  = TABLE_DATA[idx + SIZE_X];                          /* f(x  ,y+1) */
    factxy = TABLE_DATA[idx + SIZE_X+1];                        /* f(x+1,y+1) */

    /** - bilinear interpolation */
    tmp  = fact;                                                /* f(x,y) */
    tmp += (((int32)(factx - fact) * (int32)deltax) >> SFR_X);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) */
    tmp += (((int32)(facty - fact) * (int32)deltay) >> SFR_Y);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))*/
    tmp += ((((int32)fact + (int32)factxy - (int32)factx - (int32)facty) * (int32)deltaxy) >> SFR_X);   /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))  + dxy*(f(x,y) + f(x+1,y+1) - f(x+1,y) - f(x,y+1)) */

    OUT = (int16)tmp;
/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable2D_FiP16_Init(LOOKUPTABLE2D_FIP16 *pTLookupTable2D_FiP16)
{
    pTLookupTable2D_FiP16->ID = LOOKUPTABLE2D_FIP16_ID;
    pTLookupTable2D_FiP16->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP16_Load(const LOOKUPTABLE2D_FIP16 *pTLookupTable2D_FiP16, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)30 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)pTLookupTable2D_FiP16->sfrX;
        data[1] = (uint8)(pTLookupTable2D_FiP16->maskX & 0x00FF);
        data[2] = (uint8)((pTLookupTable2D_FiP16->maskX >> 8) & 0x00FF);
        data[3] = (uint8)(pTLookupTable2D_FiP16->idxOffsetX & 0x00FF);
        data[4] = (uint8)((pTLookupTable2D_FiP16->idxOffsetX >> 8) & 0x00FF);
        data[5] = (uint8)(pTLookupTable2D_FiP16->sizeX & 0x00FF);
        data[6] = (uint8)((pTLookupTable2D_FiP16->sizeX >> 8) & 0x00FF);
        data[7] = (uint8)(pTLookupTable2D_FiP16->gainX & 0x00FF);
        data[8] = (uint8)((pTLookupTable2D_FiP16->gainX >> 8) & 0x00FF);
        data[9] = (uint8)pTLookupTable2D_FiP16->gainXsfr;
        data[10] = (uint8)(pTLookupTable2D_FiP16->offsetX & 0x00FF);
        data[11] = (uint8)((pTLookupTable2D_FiP16->offsetX >> 8) & 0x00FF);
        data[12] = (uint8)(pTLookupTable2D_FiP16->minX & 0x00FF);
        data[13] = (uint8)((pTLookupTable2D_FiP16->minX >> 8) & 0x00FF);
        data[14] = (uint8)(pTLookupTable2D_FiP16->maxX & 0x00FF);
        data[15] = (uint8)((pTLookupTable2D_FiP16->maxX >> 8) & 0x00FF);
        data[16] = (uint8)pTLookupTable2D_FiP16->sfrY;
        data[17] = (uint8)(pTLookupTable2D_FiP16->maskY & 0x00FF);
        data[18] = (uint8)((pTLookupTable2D_FiP16->maskY >> 8) & 0x00FF);
        data[19] = (uint8)(pTLookupTable2D_FiP16->idxOffsetY & 0x00FF);
        data[20] = (uint8)((pTLookupTable2D_FiP16->idxOffsetY >> 8) & 0x00FF);
        data[21] = (uint8)(pTLookupTable2D_FiP16->gainY & 0x00FF);
        data[22] = (uint8)((pTLookupTable2D_FiP16->gainY >> 8) & 0x00FF);
        data[23] = (uint8)pTLookupTable2D_FiP16->gainYsfr;
        data[24] = (uint8)(pTLookupTable2D_FiP16->offsetY & 0x00FF);
        data[25] = (uint8)((pTLookupTable2D_FiP16->offsetY >> 8) & 0x00FF);
        data[26] = (uint8)(pTLookupTable2D_FiP16->minY & 0x00FF);
        data[27] = (uint8)((pTLookupTable2D_FiP16->minY >> 8) & 0x00FF);
        data[28] = (uint8)(pTLookupTable2D_FiP16->maxY & 0x00FF);
        data[29] = (uint8)((pTLookupTable2D_FiP16->maxY >> 8) & 0x00FF);
        *dataLength = (uint16)30;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP16_Save(LOOKUPTABLE2D_FIP16 *pTLookupTable2D_FiP16, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)30)
    {
        error = (uint8)1;
    }
    else
    {
        pTLookupTable2D_FiP16->sfrX = UINT8_TO_INT8((uint8)data[0]);
        pTLookupTable2D_FiP16->maskX = ((uint16)data[1] + \
            ((uint16)data[2] << 8));
        pTLookupTable2D_FiP16->idxOffsetX = ((uint16)data[3] + \
            ((uint16)data[4] << 8));
        pTLookupTable2D_FiP16->sizeX = ((uint16)data[5] + \
            ((uint16)data[6] << 8));
        pTLookupTable2D_FiP16->gainX = UINT16_TO_INT16((uint16)data[7] + \
            ((uint16)data[8] << 8));
        pTLookupTable2D_FiP16->gainXsfr = UINT8_TO_INT8((uint8)data[9]);
        pTLookupTable2D_FiP16->offsetX = UINT16_TO_INT16((uint16)data[10] + \
            ((uint16)data[11] << 8));
        pTLookupTable2D_FiP16->minX = UINT16_TO_INT16((uint16)data[12] + \
            ((uint16)data[13] << 8));
        pTLookupTable2D_FiP16->maxX = UINT16_TO_INT16((uint16)data[14] + \
            ((uint16)data[15] << 8));
        pTLookupTable2D_FiP16->sfrY = UINT8_TO_INT8((uint8)data[16]);
        pTLookupTable2D_FiP16->maskY = ((uint16)data[17] + \
            ((uint16)data[18] << 8));
        pTLookupTable2D_FiP16->idxOffsetY = ((uint16)data[19] + \
            ((uint16)data[20] << 8));
        pTLookupTable2D_FiP16->gainY = UINT16_TO_INT16((uint16)data[21] + \
            ((uint16)data[22] << 8));
        pTLookupTable2D_FiP16->gainYsfr = UINT8_TO_INT8((uint8)data[23]);
        pTLookupTable2D_FiP16->offsetY = UINT16_TO_INT16((uint16)data[24] + \
            ((uint16)data[25] << 8));
        pTLookupTable2D_FiP16->minY = UINT16_TO_INT16((uint16)data[26] + \
            ((uint16)data[27] << 8));
        pTLookupTable2D_FiP16->maxY = UINT16_TO_INT16((uint16)data[28] + \
            ((uint16)data[29] << 8));
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE2D_FIP16_ISLINKED)
void* LookupTable2D_FiP16_GetAddress(const LOOKUPTABLE2D_FIP16* block, uint16 elementId)
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
