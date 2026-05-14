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
 * $LastChangedRevision: 2945 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:  D Controller with
 *                          - Enable Input
 *                          - Initial Condition Input
 *                          - Output Limitation
 *                      Calculation ZOH:
 *                                      z - 1
 *                      y = (Kd*wc --------------- )*u
 *                                 z - exp(-wc*Ts)
 *
 *                      -> y(k) = y(k-1) + b1d.u(k) + b0d.u(k-1) - a0d.yd(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include "DLimit_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(DLimit_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*pTDLimit_FiP16->In)           /* Q15 */
#define INIT            (*pTDLimit_FiP16->D0)           /* Q15 */
#define ENABLE          (*pTDLimit_FiP16->Enable)       /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*pTDLimit_FiP16->max)          /* Q31 */
    #define LIMIT_DOWN  (*pTDLimit_FiP16->min)          /* Q31 */
#endif

/* Outputs */
#define OUT             (pTDLimit_FiP16->Out)           /* Q15 */

/* Parameter */
#define B0D             (pTDLimit_FiP16->b0d)           /* Qz */
#define B1D             (pTDLimit_FiP16->b1d)           /* Qz */
#define A0D             (pTDLimit_FiP16->a0d)           /* Q15 */

#define SFRD            (pTDLimit_FiP16->sfrd)          /* z */

/* Variables */
#define IN_OLD          (pTDLimit_FiP16->in_old)        /* Q15 */
#define ENABLE_OLD      (pTDLimit_FiP16->enable_old)
#define OUT_OLD         (OUT)                           /* Out holds the old value before it's getting updated */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void DLimit_FiP16_Update(DLIMIT_FIP16 *pTDLimit_FiP16)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 y;

    if (ENABLE) /* Block enabled */
    {
        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialize derivative part */
            IN_OLD = INIT;
        }

        /* Derivative term */
        y  =  ((int32)B0D * (int32)IN_OLD);         /* Qz */
        y += (((int32)B1D * (int32)IN));            /* Qz */
        y  = y >> SFRD;                             /* Qz -> Q15 */
        y -= (((int32)A0D * (int32)OUT_OLD) >> 15); /* Q15 */
        LIMIT(y, INT16_MAX);

        /* Output limitation */
        if (y > LIMIT_UP)           /* Output is beyond upper limit */
        {
            /* Limitation of output to upper boundary */
            y = LIMIT_UP;
        }
        else if (y < LIMIT_DOWN)    /* Output is beyond lower limit */
        {
            /* Limitation of output to lower boundary */
            y = LIMIT_DOWN;
        }
        /* else: no output limitation */

        /* Assign output */
        OUT = (int16)y;
    }
    else                /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }

    /* Save relevant data for next call */
    IN_OLD = IN;
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void DLimit_FiP16_Init(DLIMIT_FIP16 *pTDLimit_FiP16)
{
    pTDLimit_FiP16->ID = DLIMIT_FIP16_ID;
    pTDLimit_FiP16->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    IN_OLD = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP16_Load(const DLIMIT_FIP16 *pTDLimit_FiP16, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)7 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTDLimit_FiP16->b0d & 0x00FF);
        data[1] = (uint8)((pTDLimit_FiP16->b0d >> 8) & 0x00FF);
        data[2] = (uint8)(pTDLimit_FiP16->b1d & 0x00FF);
        data[3] = (uint8)((pTDLimit_FiP16->b1d >> 8) & 0x00FF);
        data[4] = (uint8)(pTDLimit_FiP16->a0d & 0x00FF);
        data[5] = (uint8)((pTDLimit_FiP16->a0d >> 8) & 0x00FF);
        data[6] = (uint8)pTDLimit_FiP16->sfrd;
        *dataLength = (uint16)7;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP16_Save(DLIMIT_FIP16 *pTDLimit_FiP16, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)7)
    {
        error = (uint8)1;
    }
    else
    {
        pTDLimit_FiP16->b0d = UINT16_TO_INT16((uint16)data[0] + \
            ((uint16)data[1] << 8));
        pTDLimit_FiP16->b1d = UINT16_TO_INT16((uint16)data[2] + \
            ((uint16)data[3] << 8));
        pTDLimit_FiP16->a0d = UINT16_TO_INT16((uint16)data[4] + \
            ((uint16)data[5] << 8));
        pTDLimit_FiP16->sfrd = UINT8_TO_INT8((uint8)data[6]);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(DLIMIT_FIP16_ISLINKED)
void* DLimit_FiP16_GetAddress(const DLIMIT_FIP16* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = (void*)block->In;
            break;
        case 2:
            addr = (void*)block->D0;
            break;
        case 3:
            addr = (void*)block->max;
            break;
        case 4:
            addr = (void*)block->min;
            break;
        case 5:
            addr = (void*)block->Enable;
            break;
        case 6:
            addr = (void*)&block->Out;
            break;
        default:
            addr = (void*)0;
            break;
    }
    return (addr);
}
#endif
