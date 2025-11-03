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
#include "TDSystemO1_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TDSystemO1_Float32_Update, "ramfuncs")
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
void TDSystemO1_Float32_Update(TDSYSTEMO1_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 temp;

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
void TDSystemO1_Float32_Init(TDSYSTEMO1_FLOAT32 *block)
{
    block->ID = TDSYSTEMO1_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset state */
     X1 = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_Float32_Load(const TDSYSTEMO1_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 16U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        (void) memcpy((void*)&tmp32, (const void*)&block->a11, sizeof(uint32));
        data[0] = (uint8)(tmp32 & 0xFFU);
        data[1] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->b11, sizeof(uint32));
        data[4] = (uint8)(tmp32 & 0xFFU);
        data[5] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[6] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[7] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->c11, sizeof(uint32));
        data[8] = (uint8)(tmp32 & 0xFFU);
        data[9] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->d11, sizeof(uint32));
        data[12] = (uint8)(tmp32 & 0xFFU);
        data[13] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[14] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[15] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 16U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_Float32_Save(TDSYSTEMO1_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 16U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        tmp32 = (uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24));
        (void) memcpy((void*)&block->a11, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24));
        (void) memcpy((void*)&block->b11, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24));
        (void) memcpy((void*)&block->c11, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24));
        (void) memcpy((void*)&block->d11, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TDSYSTEMO1_FLOAT32_ISLINKED)
void* TDSystemO1_Float32_GetAddress(TDSYSTEMO1_FLOAT32* block, uint16 elementId)
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
