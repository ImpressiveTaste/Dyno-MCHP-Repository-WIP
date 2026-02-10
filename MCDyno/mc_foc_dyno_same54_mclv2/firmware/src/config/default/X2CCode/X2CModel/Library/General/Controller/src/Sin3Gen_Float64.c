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
/*     Description:    Sine Wave Generation for 3 Phase System
 *                     Calculation:
 *                          u =  A*sin(phi(t)) + Offset
 *                          v =  A*sin(phi(t) - 2*pi/3) + Offset
 *                          w =  A*sin(phi(t) + 2*pi/3) + Offset
 *                          phi(t) = phi(t-1) + 2*pi*Ts*f
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Sin3Gen_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Sin3Gen_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include <math.h>

/* Inputs */
#define AMPLITUDE       (*block->A)
#define FREQUENCY       (*block->f)

/* Outputs */
#define OUT_U           (block->u)
#define OUT_V           (block->v)
#define OUT_W           (block->w)

/* Parameter */
#define OFFSET          (block->offset)
#define DELTA_PHI       (block->delta_phi)

/* Variables */
#define PHI             (block->phi)

/* Constants */
#define TWOPITHIRD      (PI_R64 * (float64)2.0/(float64)3.0)                /* 2*Pi/3 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Sin3Gen_Float64_Update(SIN3GEN_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 sine, out1, out2, angle;
    float64 temp;

    temp = DELTA_PHI * FREQUENCY;           /* calculate angle increment */
    PHI += temp;                            /* increment angle */

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

    /*------------------------------------------------------------------------*/
    /* Calculation of phase U                                                 */
    /*------------------------------------------------------------------------*/
    /* sine calculation */
    sine = sin(PHI);                        /* sin(phi) */

    /* amplification */
    temp = AMPLITUDE * sine;                /* A*sin(phi) */

    /* offset */
    temp += OFFSET;                         /* A*sin(phi) + Offset */

    out1 = temp;

    /*------------------------------------------------------------------------*/
    /* Calculation of phase V                                                 */
    /*------------------------------------------------------------------------*/
    /* sine interpolation */
    angle = PHI - TWOPITHIRD;               /* phi - 2*pi/3 (though angle might exceeds -pi..pi range, sine will be computed correctly) */
    sine = sin(angle);                      /* sin(phi - 2*pi/3) */

    /* amplification */
    temp = AMPLITUDE * sine;                /* A*sin(phi - 2*pi/3) */

    /* offset */
    temp += OFFSET;                         /* A*sin(phi - 2*pi/3) + Offset*/

    out2 = temp;

    /*------------------------------------------------------------------------*/
    /* Calculation of phase W                                                 */
    /*------------------------------------------------------------------------*/
    /* sine interpolation */
    angle = PHI + TWOPITHIRD;               /* phi + 2*pi/3 (though angle might exceeds -pi..pi range, sine will be computed correctly) */
    sine = sin(angle);                      /* sin(phi + 2*pi/3) */

    /* amplification */
    temp = AMPLITUDE * sine;                /* A*sin(phi + 2*pi/3) */

    /* offset */
    temp += OFFSET;                         /* A*sin(phi + 2*pi/3) + Offset*/

    /*------------------------------------------------------------------------*/
    /* Assigning of outputs                                                   */
    /*------------------------------------------------------------------------*/
    OUT_U = out1;
    OUT_V = out2;
    OUT_W = temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Sin3Gen_Float64_Init(SIN3GEN_FLOAT64 *block)
{
    block->ID = SIN3GEN_FLOAT64_ID;
    block->u = 0.0;
    block->v = 0.0;
    block->w = 0.0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    PHI = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin3Gen_Float64_Load(const SIN3GEN_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 16U)
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
        (void) memcpy((void*)&tmp64, (const void*)&block->offset, sizeof(uint64));
        data[8] = (uint8)(tmp64 & 0xFFU);
        data[9] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[12] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[13] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[14] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[15] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 16U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin3Gen_Float64_Save(SIN3GEN_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 16U)
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
#if !defined(SIN3GEN_FLOAT64_ISLINKED)
void* Sin3Gen_Float64_GetAddress(SIN3GEN_FLOAT64* block, uint16 elementId)
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
        case 4:
            addr = &block->v;
            break;
        case 5:
            addr = &block->w;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
