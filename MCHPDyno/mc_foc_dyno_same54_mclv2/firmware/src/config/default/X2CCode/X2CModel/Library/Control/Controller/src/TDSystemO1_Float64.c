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
/**     Description: 1st Order Time Discrete System                                                                  **/
/**                  Calculation:                                                                                    **/
/**                     x(k+1) = a11*x(k) + b11*u(k)                                                                 **/
/**                     y(k)   = c11*x(k) + d11*u(k)                                                                 **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TDSystemO1_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TDSystemO1_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN      (*block->In)

/* Outputs */
#define OUT     (block->Out)

/* Matrices */
#define A11     (block->a11)
#define B11     (block->b11)
#define C11     (block->c11)
#define D11     (block->d11)


/* State */
#define X1      (block->x1)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TDSystemO1_Float64_Update(TDSYSTEMO1_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 temp;

    /* Calculation of output (with old state) */
    temp  =  C11 * X1;      /* y = c11*x1 */
    temp += (D11 * IN);     /* y = c11*x1 + d11*u */
    OUT   = temp;           /* assign output  */

    /* Calculation of new state */
    temp  =  A11 * X1;      /* x1 = a11*x1 */
    temp += (B11 * IN);     /* x1 = a11*x1 + b11*u */
    X1    = temp;           /* assign state */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TDSystemO1_Float64_Init(TDSYSTEMO1_FLOAT64 *block)
{
    block->ID = TDSYSTEMO1_FLOAT64_ID;
    block->Out = 0.0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset state */
     X1 = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_Float64_Load(const TDSYSTEMO1_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 32U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        (void) memcpy((void*)&tmp64, (const void*)&block->a11, sizeof(uint64));
        data[0] = (uint8)(tmp64 & 0xFFU);
        data[1] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[4] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[5] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[6] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[7] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->b11, sizeof(uint64));
        data[8] = (uint8)(tmp64 & 0xFFU);
        data[9] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[12] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[13] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[14] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[15] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->c11, sizeof(uint64));
        data[16] = (uint8)(tmp64 & 0xFFU);
        data[17] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[20] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[21] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[22] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[23] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->d11, sizeof(uint64));
        data[24] = (uint8)(tmp64 & 0xFFU);
        data[25] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[26] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[27] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[28] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[29] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[30] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[31] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 32U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_Float64_Save(TDSYSTEMO1_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 32U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        tmp64 = (uint64)((uint64)data[0] | ((uint64)data[1] << 8) | ((uint64)data[2] << 16) | ((uint64)data[3] << 24) | 
            ((uint64)data[4] << 32) | ((uint64)data[5] << 40) | ((uint64)data[6] << 48) | ((uint64)data[7] << 56));
        (void) memcpy((void*)&block->a11, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[8] | ((uint64)data[9] << 8) | ((uint64)data[10] << 16) | ((uint64)data[11] << 24) | 
            ((uint64)data[12] << 32) | ((uint64)data[13] << 40) | ((uint64)data[14] << 48) | ((uint64)data[15] << 56));
        (void) memcpy((void*)&block->b11, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[16] | ((uint64)data[17] << 8) | ((uint64)data[18] << 16) | ((uint64)data[19] << 24) | 
            ((uint64)data[20] << 32) | ((uint64)data[21] << 40) | ((uint64)data[22] << 48) | ((uint64)data[23] << 56));
        (void) memcpy((void*)&block->c11, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[24] | ((uint64)data[25] << 8) | ((uint64)data[26] << 16) | ((uint64)data[27] << 24) | 
            ((uint64)data[28] << 32) | ((uint64)data[29] << 40) | ((uint64)data[30] << 48) | ((uint64)data[31] << 56));
        (void) memcpy((void*)&block->d11, (const void*)&tmp64, sizeof(float64));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TDSYSTEMO1_FLOAT64_ISLINKED)
void* TDSystemO1_Float64_GetAddress(TDSYSTEMO1_FLOAT64* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
