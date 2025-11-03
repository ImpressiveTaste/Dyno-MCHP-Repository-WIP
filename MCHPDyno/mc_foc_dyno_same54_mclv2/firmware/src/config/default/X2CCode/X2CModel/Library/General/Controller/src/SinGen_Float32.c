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
#include "SinGen_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(SinGen_Float32_Update, "ramfuncs")
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
void SinGen_Float32_Update(SINGEN_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 sine, angle;
    float32 temp;

    temp = DELTA_PHI * FREQUENCY;           /* calculate angle increment */
    PHI += temp;                            /* increment angle */

    /* angle limitation to -pi..+pi range */
    if (PHI >= PI_R32)
    {
        PHI = PHI - (2 * PI_R32);
    }
    else if (PHI < -PI_R32)
    {
        PHI = PHI + (2 * PI_R32);
    }
    else
    {
        /* else: No operation required */
    }

    /* sine calculation */
    angle = PHI + PHASE;                    /* phi+phase (though angle might exceeds -pi..pi range, sine will be computed correctly) */
    sine  = sinf(angle);                    /* sin(phi+phase) */

    /* amplification */
    temp = AMPLITUDE * sine;                /* A*sin(phi+phase) */

    /* offset */
    temp += OFFSET;                         /* A*sin(phi+phase) + Offset */

    OUT = temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void SinGen_Float32_Init(SINGEN_FLOAT32 *block)
{
    block->ID = SINGEN_FLOAT32_ID;
    block->u = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    PHI = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_Float32_Load(const SINGEN_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 12U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        (void) memcpy((void*)&tmp32, (const void*)&block->delta_phi, sizeof(uint32));
        data[0] = (uint8)(tmp32 & 0xFFU);
        data[1] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->phase, sizeof(uint32));
        data[4] = (uint8)(tmp32 & 0xFFU);
        data[5] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[6] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[7] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->offset, sizeof(uint32));
        data[8] = (uint8)(tmp32 & 0xFFU);
        data[9] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 12U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_Float32_Save(SINGEN_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 12U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        tmp32 = (uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24));
        (void) memcpy((void*)&block->delta_phi, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24));
        (void) memcpy((void*)&block->phase, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24));
        (void) memcpy((void*)&block->offset, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SINGEN_FLOAT32_ISLINKED)
void* SinGen_Float32_GetAddress(SINGEN_FLOAT32* block, uint16 elementId)
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
