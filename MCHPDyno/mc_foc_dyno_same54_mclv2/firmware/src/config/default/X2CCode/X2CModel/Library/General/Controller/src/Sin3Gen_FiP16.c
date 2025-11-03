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
#include "Sin3Gen_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Sin3Gen_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include "Sin_Data.h"

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
#define TWOPITHIRDINT16 0x5555              /* 2*Pi/3 in Q15 format */
#define ROUNDING        0x4000              /* value used for rounding */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Sin3Gen_FiP16_Update(SIN3GEN_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16   sine, out1, out2, angle;
    int32   temp;

    temp  = (int32)DELTA_PHI * (int32)FREQUENCY;    /* calculate angle increment */
    temp += ROUNDING;
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    PHI  += ((int16)(temp >> 15));                  /* scale to 16 Bit (Q30 -> Q15) and increment angle */

    /*------------------------------------------------------------------------*/
    /* Calculation of phase U                                                 */
    /*------------------------------------------------------------------------*/
    /* sine interpolation */
    sine = lookupTableInt16(Sin_Table16, PHI);      /* sin(phi) */
    /* amplification */
    temp = (int32)AMPLITUDE * (int32)sine;          /* A*sin(phi) */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    temp = (temp + ROUNDING) >> 15;
    /* offset */
    temp += (int32)OFFSET;                          /* A*sin(phi) + Offset*/

    /* Data width limitation, previous: LIMIT(temp, INT16_MAX);*/
    if (temp > INT16_MAX)
    {
        temp = INT16_MAX;
    }
    else
    {
        if (temp < -INT16_MAX)
        {
            temp = -INT16_MAX;
        }
    }
    out1 = (int16)temp;

    /*------------------------------------------------------------------------*/
    /* Calculation of phase V                                                 */
    /*------------------------------------------------------------------------*/
    /* sine interpolation */
    angle = PHI - TWOPITHIRDINT16;                  /* phi - 2*pi/3 */
    sine = lookupTableInt16(Sin_Table16, angle);      /* sin(phi - 2*pi/3) */
    /* amplification */
    temp = (int32)AMPLITUDE * (int32)sine;          /* A*sin(phi - 2*pi/3) */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    temp = (temp + ROUNDING) >> 15;
    /* offset */
    temp += (int32)OFFSET;                          /* A*sin(phi - 2*pi/3) + Offset*/

    /* Data width limitation, previous: LIMIT(temp, INT16_MAX);*/
        if (temp > INT16_MAX)
        {
            temp = INT16_MAX;
        }
        else
        {
            if (temp < -INT16_MAX)
            {
                temp = -INT16_MAX;
            }
        }
    out2 = (int16)temp;

    /*------------------------------------------------------------------------*/
    /* Calculation of phase W                                                 */
    /*------------------------------------------------------------------------*/
    /* sine interpolation */
    angle = PHI + TWOPITHIRDINT16;                  /* phi + 2*pi/3 */
    sine = lookupTableInt16(Sin_Table16, angle);      /* sin(phi + 2*pi/3) */
    /* amplification */
    temp = (int32)AMPLITUDE * (int32)sine;          /* A*sin(phi + 2*pi/3) */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    temp = (temp + ROUNDING) >> 15;
    /* offset */
    temp += (int32)OFFSET;                          /* A*sin(phi + 2*pi/3) + Offset*/

    /* Data width limitation, previous: LIMIT(temp, INT16_MAX);*/
    if (temp > INT16_MAX)
    {
        temp = INT16_MAX;
    }
    else
    {
        if (temp < -INT16_MAX)
        {
            temp = -INT16_MAX;
        }
    }

    /*------------------------------------------------------------------------*/
    /* Assigning of (remaining) outputs                                       */
    /*------------------------------------------------------------------------*/
    OUT_U = out1;
    OUT_V = out2;
    OUT_W = (int16)temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Sin3Gen_FiP16_Init(SIN3GEN_FIP16 *block)
{
    block->ID = SIN3GEN_FIP16_ID;
    block->u = 0;
    block->v = 0;
    block->w = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    PHI = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin3Gen_FiP16_Load(const SIN3GEN_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 4U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->delta_phi & 0xFFU);
        data[1] = (uint8)(((uint16)block->delta_phi >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->offset & 0xFFU);
        data[3] = (uint8)(((uint16)block->offset >> 8) & 0xFFU);
        *dataLength = 4U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin3Gen_FiP16_Save(SIN3GEN_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 4U)
    {
        error = 1U;
    }
    else
    {
        block->delta_phi = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->offset = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SIN3GEN_FIP16_ISLINKED)
void* Sin3Gen_FiP16_GetAddress(SIN3GEN_FIP16* block, uint16 elementId)
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
