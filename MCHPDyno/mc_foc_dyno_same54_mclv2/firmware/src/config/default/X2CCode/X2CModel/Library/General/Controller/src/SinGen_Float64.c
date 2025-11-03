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
/*      Description:    Sine Wave Generation
 *                     Calculation:
 *                          u =  A*sin(phi(t)+Phase) + Offset
 *                          phi(t) = phi(t-1) + 2*pi*Ts*f
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "SinGen_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(SinGen_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include <math.h>

/* Input */
#define AMPLITUDE   (*block->A)
#define FREQUENCY   (*block->f)

/* Output */
#define OUT         (block->u)

/* Parameter */
#define OFFSET      (block->offset)
#define PHASE       (block->phase)
#define DELTA_PHI   (block->delta_phi)

/* Variables */
#define PHI         (block->phi)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void SinGen_Float64_Update(SINGEN_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 sine, angle;
    float64 temp;

    temp = DELTA_PHI * FREQUENCY;   /* calculate angle increment */
    PHI += temp;                    /* increment angle */

    /* angle limitation to -pi..+pi range */
    if (PHI >= PI_R64)
    {
        PHI = PHI - (2 * PI_R64);
    }
    else if (PHI < -PI_R64)
    {
        PHI = PHI + (2 * PI_R64);
    }
    else
    {
        /* else: No operation required */
    }

    /* sine calculation */
    angle = PHI + PHASE;            /* phi+phase (though angle might exceeds -pi..pi range, sine will be computed correctly) */
    sine  = sin(angle);             /* sin(phi+phase) */

    /* amplification */
    temp = AMPLITUDE * sine;        /* A*sin(phi+phase) */

    /* offset */
    temp += OFFSET;                 /* A*sin(phi+phase) + Offset */

    OUT = temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void SinGen_Float64_Init(SINGEN_FLOAT64 *block)
{
    block->ID = SINGEN_FLOAT64_ID;
    block->u = 0.0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    PHI = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_Float64_Load(const SINGEN_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 24U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        (void) memcpy((void*)&tmp64, (const void*)&block->delta_phi, sizeof(uint64));
        data[0] = (uint8)(tmp64 & 0xFFU);
        data[1] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[4] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[5] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[6] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[7] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->phase, sizeof(uint64));
        data[8] = (uint8)(tmp64 & 0xFFU);
        data[9] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[12] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[13] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[14] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[15] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->offset, sizeof(uint64));
        data[16] = (uint8)(tmp64 & 0xFFU);
        data[17] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[20] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[21] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[22] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[23] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 24U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_Float64_Save(SINGEN_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 24U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        tmp64 = (uint64)((uint64)data[0] | ((uint64)data[1] << 8) | ((uint64)data[2] << 16) | ((uint64)data[3] << 24) | 
            ((uint64)data[4] << 32) | ((uint64)data[5] << 40) | ((uint64)data[6] << 48) | ((uint64)data[7] << 56));
        (void) memcpy((void*)&block->delta_phi, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[8] | ((uint64)data[9] << 8) | ((uint64)data[10] << 16) | ((uint64)data[11] << 24) | 
            ((uint64)data[12] << 32) | ((uint64)data[13] << 40) | ((uint64)data[14] << 48) | ((uint64)data[15] << 56));
        (void) memcpy((void*)&block->phase, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[16] | ((uint64)data[17] << 8) | ((uint64)data[18] << 16) | ((uint64)data[19] << 24) | 
            ((uint64)data[20] << 32) | ((uint64)data[21] << 40) | ((uint64)data[22] << 48) | ((uint64)data[23] << 56));
        (void) memcpy((void*)&block->offset, (const void*)&tmp64, sizeof(float64));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SINGEN_FLOAT64_ISLINKED)
void* SinGen_Float64_GetAddress(SINGEN_FLOAT64* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->A;
            break;
        case 2:
            addr = block->f;
            break;
        case 3:
            addr = &block->u;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
