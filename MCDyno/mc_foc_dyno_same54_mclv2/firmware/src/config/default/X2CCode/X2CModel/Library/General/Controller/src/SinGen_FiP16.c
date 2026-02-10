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
#include "SinGen_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(SinGen_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include "Sin_Data.h"

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

/* Constants */
#define ROUNDING    0x4000              /* value used for rounding */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void SinGen_FiP16_Update(SINGEN_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16   sine, angle;
    int32   temp;

    temp  = (int32)DELTA_PHI * (int32)FREQUENCY;    /* calculate angle increment */
    temp += ROUNDING;

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    PHI  += ((int16)(temp >> 15));                  /* scale to 16 Bit (Q30 -> Q15) and increment angle */

    /* sine interpolation */
    angle = PHI + PHASE;                        /* phi+phase */
    sine = lookupTableInt16(Sin_Table16, angle);      /* sin(phi+phase) */
    /* amplification */
    temp  = (int32)AMPLITUDE * (int32)sine;     /* A*sin(phi+phase) */

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002 ", true_no_defect) */
    temp  = (temp + ROUNDING) >> 15;            /* round and convert from Q30 to Q15 */
    /* offset */
    temp += (int32)OFFSET;                      /* A*sin(phi+phase) + Offset */

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

    OUT = (int16)temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void SinGen_FiP16_Init(SINGEN_FIP16 *block)
{
    block->ID = SINGEN_FIP16_ID;
    block->u = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    PHI = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_FiP16_Load(const SINGEN_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 6U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->delta_phi & 0xFFU);
        data[1] = (uint8)(((uint16)block->delta_phi >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->phase & 0xFFU);
        data[3] = (uint8)(((uint16)block->phase >> 8) & 0xFFU);
        data[4] = (uint8)((uint16)block->offset & 0xFFU);
        data[5] = (uint8)(((uint16)block->offset >> 8) & 0xFFU);
        *dataLength = 6U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 SinGen_FiP16_Save(SINGEN_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 6U)
    {
        error = 1U;
    }
    else
    {
        block->delta_phi = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->phase = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        block->offset = castUint16ToInt16((uint16)((uint16)data[4] | ((uint16)data[5] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SINGEN_FIP16_ISLINKED)
void* SinGen_FiP16_GetAddress(SINGEN_FIP16* block, uint16 elementId)
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
