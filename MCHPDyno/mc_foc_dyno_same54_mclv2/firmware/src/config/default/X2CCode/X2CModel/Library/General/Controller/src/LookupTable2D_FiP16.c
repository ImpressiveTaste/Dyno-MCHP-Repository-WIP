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
#include "LookupTable2D_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(LookupTable2D_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN_X            (*block->x)
#define IN_Y            (*block->y)

/* Outputs */
#define OUT             (block->Out)

/* Parameters */
#define TABLE_DATA      (block->Table)
#define SIZE_X          (block->sizeX)

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



/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void LookupTable2D_FiP16_Update(LOOKUPTABLE2D_FIP16 *block)
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

    /** - calculate table indexes */
    /* RULECHECKER_comment(1:0, 2:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    idxx   = (inx >> SFR_X) + IDX_OFFSET_X;    /*  x  */
    idxy   = (iny >> SFR_Y) + IDX_OFFSET_Y;    /*  y  */
    idx    = idxx + (idxy*SIZE_X);

    /** - calculate fractual part */
    deltax  = inx & MASK_X;                                     /* dx  */
    deltay  = iny & MASK_Y;                                     /* dy  */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    deltaxy = ((int32)(deltax)*(int32)(deltay)) >> SFR_Y;       /* dxy */

    /** - calculate value at lattice points */
    fact   = TABLE_DATA[idx];                                   /* f(x  ,y)   */
    factx  = TABLE_DATA[idx + 1];                               /* f(x+1,y)   */
    facty  = TABLE_DATA[idx + SIZE_X];                          /* f(x  ,y+1) */
    factxy = TABLE_DATA[idx + SIZE_X+1];                        /* f(x+1,y+1) */

    /** - bilinear interpolation */
    tmp  = fact;                                                /* f(x,y) */
    /* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    tmp += (((int32)(factx - fact) * (int32)deltax) >> SFR_X);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) */
    tmp += (((int32)(facty - fact) * (int32)deltay) >> SFR_Y);  /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))*/
    tmp += ((((int32)fact + (int32)factxy - (int32)factx - (int32)facty) * (int32)deltaxy) >> SFR_X);   /* f(x,y) + dx*(f(x+1,y) - f(x,y)) + dy*(f(x,y+1) - f(x,y))  + dxy*(f(x,y) + f(x+1,y+1) - f(x+1,y) - f(x,y+1)) */

    OUT = (int16)tmp;
/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void LookupTable2D_FiP16_Init(LOOKUPTABLE2D_FIP16 *block)
{
    block->ID = LOOKUPTABLE2D_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP16_Load(const LOOKUPTABLE2D_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 30U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->sfrX & 0xFFU);
        data[1] = (uint8)(block->maskX & 0xFFU);
        data[2] = (uint8)((block->maskX >> 8) & 0xFFU);
        data[3] = (uint8)(block->idxOffsetX & 0xFFU);
        data[4] = (uint8)((block->idxOffsetX >> 8) & 0xFFU);
        data[5] = (uint8)(block->sizeX & 0xFFU);
        data[6] = (uint8)((block->sizeX >> 8) & 0xFFU);
        data[7] = (uint8)((uint16)block->gainX & 0xFFU);
        data[8] = (uint8)(((uint16)block->gainX >> 8) & 0xFFU);
        data[9] = (uint8)(block->gainXsfr & 0xFFU);
        data[10] = (uint8)((uint16)block->offsetX & 0xFFU);
        data[11] = (uint8)(((uint16)block->offsetX >> 8) & 0xFFU);
        data[12] = (uint8)((uint16)block->minX & 0xFFU);
        data[13] = (uint8)(((uint16)block->minX >> 8) & 0xFFU);
        data[14] = (uint8)((uint16)block->maxX & 0xFFU);
        data[15] = (uint8)(((uint16)block->maxX >> 8) & 0xFFU);
        data[16] = (uint8)(block->sfrY & 0xFFU);
        data[17] = (uint8)(block->maskY & 0xFFU);
        data[18] = (uint8)((block->maskY >> 8) & 0xFFU);
        data[19] = (uint8)(block->idxOffsetY & 0xFFU);
        data[20] = (uint8)((block->idxOffsetY >> 8) & 0xFFU);
        data[21] = (uint8)((uint16)block->gainY & 0xFFU);
        data[22] = (uint8)(((uint16)block->gainY >> 8) & 0xFFU);
        data[23] = (uint8)(block->gainYsfr & 0xFFU);
        data[24] = (uint8)((uint16)block->offsetY & 0xFFU);
        data[25] = (uint8)(((uint16)block->offsetY >> 8) & 0xFFU);
        data[26] = (uint8)((uint16)block->minY & 0xFFU);
        data[27] = (uint8)(((uint16)block->minY >> 8) & 0xFFU);
        data[28] = (uint8)((uint16)block->maxY & 0xFFU);
        data[29] = (uint8)(((uint16)block->maxY >> 8) & 0xFFU);
        *dataLength = 30U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 LookupTable2D_FiP16_Save(LOOKUPTABLE2D_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 30U)
    {
        error = 1U;
    }
    else
    {
        block->sfrX = data[0];
        block->maskX = (uint16)((uint16)data[1] | ((uint16)data[2] << 8));
        block->idxOffsetX = (uint16)((uint16)data[3] | ((uint16)data[4] << 8));
        block->sizeX = (uint16)((uint16)data[5] | ((uint16)data[6] << 8));
        block->gainX = castUint16ToInt16((uint16)((uint16)data[7] | ((uint16)data[8] << 8)));
        block->gainXsfr = data[9];
        block->offsetX = castUint16ToInt16((uint16)((uint16)data[10] | ((uint16)data[11] << 8)));
        block->minX = castUint16ToInt16((uint16)((uint16)data[12] | ((uint16)data[13] << 8)));
        block->maxX = castUint16ToInt16((uint16)((uint16)data[14] | ((uint16)data[15] << 8)));
        block->sfrY = data[16];
        block->maskY = (uint16)((uint16)data[17] | ((uint16)data[18] << 8));
        block->idxOffsetY = (uint16)((uint16)data[19] | ((uint16)data[20] << 8));
        block->gainY = castUint16ToInt16((uint16)((uint16)data[21] | ((uint16)data[22] << 8)));
        block->gainYsfr = data[23];
        block->offsetY = castUint16ToInt16((uint16)((uint16)data[24] | ((uint16)data[25] << 8)));
        block->minY = castUint16ToInt16((uint16)((uint16)data[26] | ((uint16)data[27] << 8)));
        block->maxY = castUint16ToInt16((uint16)((uint16)data[28] | ((uint16)data[29] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(LOOKUPTABLE2D_FIP16_ISLINKED)
void* LookupTable2D_FiP16_GetAddress(LOOKUPTABLE2D_FIP16* block, uint16 elementId)
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
