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
#include "DLimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(DLimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*pTDLimit_FiP32->In)           /* Q31 */
#define INIT            (*pTDLimit_FiP32->D0)           /* Q31 */
#define ENABLE          (*pTDLimit_FiP32->Enable)       /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*pTDLimit_FiP32->max)          /* Q62 */
    #define LIMIT_DOWN  (*pTDLimit_FiP32->min)          /* Q62 */
#endif

/* Outputs */
#define OUT             (pTDLimit_FiP32->Out)           /* Q31 */

/* Parameter */
#define B0D             (pTDLimit_FiP32->b0d)           /* Qz */
#define B1D             (pTDLimit_FiP32->b1d)           /* Qz */
#define A0D             (pTDLimit_FiP32->a0d)           /* Q31 */

#define SFRD            (pTDLimit_FiP32->sfrd)          /* z */

/* Variables */
#define IN_OLD          (pTDLimit_FiP32->in_old)        /* Q31 */
#define ENABLE_OLD      (pTDLimit_FiP32->enable_old)
#define OUT_OLD         (OUT)                           /* Out holds the old value before it's getting updated */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void DLimit_FiP32_Update(DLIMIT_FIP32 *pTDLimit_FiP32)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y;

    if (ENABLE) /* Block enabled */
    {
        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialize derivative part */
            IN_OLD = INIT;
        }

        /* Derivative term */
        y  =  ((int64)B0D * (int64)IN_OLD);         /* Qz */
        y += (((int64)B1D * (int64)IN));            /* Qz */
        y  = y >> SFRD;                             /* Qz -> Q31 */
        y -= (((int64)A0D * (int64)OUT_OLD) >> 31); /* Q31 */
        LIMIT(y, INT32_MAX);

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
        OUT = (int32)y;
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
void DLimit_FiP32_Init(DLIMIT_FIP32 *pTDLimit_FiP32)
{
    pTDLimit_FiP32->ID = DLIMIT_FIP32_ID;
    pTDLimit_FiP32->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    IN_OLD = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP32_Load(const DLIMIT_FIP32 *pTDLimit_FiP32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)13 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTDLimit_FiP32->b0d & 0x000000FF);
        data[1] = (uint8)((pTDLimit_FiP32->b0d >> 8) & 0x000000FF);
        data[2] = (uint8)((pTDLimit_FiP32->b0d >> 16) & 0x000000FF);
        data[3] = (uint8)((pTDLimit_FiP32->b0d >> 24) & 0x000000FF);
        data[4] = (uint8)(pTDLimit_FiP32->b1d & 0x000000FF);
        data[5] = (uint8)((pTDLimit_FiP32->b1d >> 8) & 0x000000FF);
        data[6] = (uint8)((pTDLimit_FiP32->b1d >> 16) & 0x000000FF);
        data[7] = (uint8)((pTDLimit_FiP32->b1d >> 24) & 0x000000FF);
        data[8] = (uint8)(pTDLimit_FiP32->a0d & 0x000000FF);
        data[9] = (uint8)((pTDLimit_FiP32->a0d >> 8) & 0x000000FF);
        data[10] = (uint8)((pTDLimit_FiP32->a0d >> 16) & 0x000000FF);
        data[11] = (uint8)((pTDLimit_FiP32->a0d >> 24) & 0x000000FF);
        data[12] = (uint8)pTDLimit_FiP32->sfrd;
        *dataLength = (uint16)13;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP32_Save(DLIMIT_FIP32 *pTDLimit_FiP32, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)13)
    {
        error = (uint8)1;
    }
    else
    {
        pTDLimit_FiP32->b0d = UINT32_TO_INT32((uint32)data[0] + \
            ((uint32)data[1] << 8) + ((uint32)data[2] << 16) + \
            ((uint32)data[3] << 24));
        pTDLimit_FiP32->b1d = UINT32_TO_INT32((uint32)data[4] + \
            ((uint32)data[5] << 8) + ((uint32)data[6] << 16) + \
            ((uint32)data[7] << 24));
        pTDLimit_FiP32->a0d = UINT32_TO_INT32((uint32)data[8] + \
            ((uint32)data[9] << 8) + ((uint32)data[10] << 16) + \
            ((uint32)data[11] << 24));
        pTDLimit_FiP32->sfrd = UINT8_TO_INT8((uint8)data[12]);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(DLIMIT_FIP32_ISLINKED)
void* DLimit_FiP32_GetAddress(const DLIMIT_FIP32* block, uint16 elementId)
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
