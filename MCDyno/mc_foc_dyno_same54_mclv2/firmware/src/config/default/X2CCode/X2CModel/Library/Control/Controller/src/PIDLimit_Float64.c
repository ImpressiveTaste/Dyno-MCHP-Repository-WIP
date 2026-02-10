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
 * $LastChangedRevision: 3575 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:  PID Controller with
 *                          - Enable Input
 *                          - Anti Windup
 *                          - Initial Condition Input
 *                          - Output Limitation
 *                      Calculation ZOH:
 *                                        1                 z - 1
 *                      y = (Kp + Ki*Ts ----- + Kd*wc --------------- )*u
 *                                      z - 1         z - exp(-wc*Ts)
 *
 *                      -> y(k) = b1.u(k) + b0.u(k-1) + y(k-1) + b1d.u(k) + b0d.u(k-1) - a0d.yd(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "PIDLimit_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PIDLimit_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)
#define ENABLE          (*block->Enable)
#define INIT            (*block->Init)
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)
    #define LIMIT_DOWN  (*block->min)
#endif

/* Outputs */
#define OUT             (block->Out)

/* Parameter */
#define B0              (block->b0)
#define B1              (block->b1)
#define B0D             (block->b0d)
#define B1D             (block->b1d)
#define A0D             (block->a0d)

/* Variables */
#define I_OLD           (block->i_old)
#define IN_OLD          (block->in_old)
#define D_OLD           (block->d_old)
#define ENABLE_OLD      (block->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PIDLimit_Float64_Update(PIDLIMIT_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 y, yp, yd;
    bool clamp;             /* Flag used for anti windup strategy clamping */

    if (ENABLE) /* Block enabled */
    {
        /* Proportional term */
        yp = B1 * IN;

        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialization of previous values */
            IN_OLD = 0;
            D_OLD  = 0;
        
            /* Calculation of initial derivative term */
            yd = (B1D * IN);    /* shorter calculation, since other differential terms equal zero */

            /* Check if intial output is out of limits */
            if (INIT > LIMIT_UP)
            {
                y = (float64)LIMIT_UP;
            }
            else if (INIT < LIMIT_DOWN)
            {
                y = (float64)LIMIT_DOWN;
            }
            else
            {
                y = (float64)INIT;
            }

            /* Initialization of integral part so that first y = INIT */
            I_OLD = y - yp - yd;
        } 
        else
        {
            /* Calculation of derivative term */
            yd  =  B0D * IN_OLD;
            yd += (B1D * IN);
            yd -= (A0D * D_OLD);
        }

        /* Sum */
        y = yp + yd + I_OLD;

        /* Output limitation and anti windup clamping */
        if (y > LIMIT_UP)
        {
            /* Output beyond upper limit */
            y = LIMIT_UP;
            
            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN > (float64)0.0);
        }
        else if (y < LIMIT_DOWN)
        {
            /* Output beyond lower limit */
            y = LIMIT_DOWN;

            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN < (float64)0.0);
        }
        /* else: no output limitation */
        else
        {
            /* Do not clamp the integrator */
            clamp = false;
        }

        /* Conditional integration = check clamping condition */   
        if (!clamp)
        {
            /* Calculation of integral part */
            I_OLD += (B0 * IN);
        }

        /* Assign output */
        OUT = y;

        /* Save relevant data for next call */
        IN_OLD = IN;
        D_OLD  = yd;
    }
    else                /* Block disabled */
    {
        OUT = (float64)0.0; /* Reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PIDLimit_Float64_Init(PIDLIMIT_FLOAT64 *block)
{
    block->ID = PIDLIMIT_FLOAT64_ID;
    block->Out = 0.0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    I_OLD  = 0;
    IN_OLD = 0;
    D_OLD  = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PIDLimit_Float64_Load(const PIDLIMIT_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 40u)
    {
        error = 1u;
    }
    else
    {
        uint64 tmp64;
        (void) memcpy((void*)&tmp64, (const void*)&block->b0, sizeof(uint64));
        data[0] = (uint8)(tmp64 & 0xFFu);
        data[1] = (uint8)((tmp64 >> 8) & 0xFFu);
        data[2] = (uint8)((tmp64 >> 16) & 0xFFu);
        data[3] = (uint8)((tmp64 >> 24) & 0xFFu);
        data[4] = (uint8)((tmp64 >> 32) & 0xFFu);
        data[5] = (uint8)((tmp64 >> 40) & 0xFFu);
        data[6] = (uint8)((tmp64 >> 48) & 0xFFu);
        data[7] = (uint8)((tmp64 >> 56) & 0xFFu);
        (void) memcpy((void*)&tmp64, (const void*)&block->b1, sizeof(uint64));
        data[8] = (uint8)(tmp64 & 0xFFu);
        data[9] = (uint8)((tmp64 >> 8) & 0xFFu);
        data[10] = (uint8)((tmp64 >> 16) & 0xFFu);
        data[11] = (uint8)((tmp64 >> 24) & 0xFFu);
        data[12] = (uint8)((tmp64 >> 32) & 0xFFu);
        data[13] = (uint8)((tmp64 >> 40) & 0xFFu);
        data[14] = (uint8)((tmp64 >> 48) & 0xFFu);
        data[15] = (uint8)((tmp64 >> 56) & 0xFFu);
        (void) memcpy((void*)&tmp64, (const void*)&block->b0d, sizeof(uint64));
        data[16] = (uint8)(tmp64 & 0xFFu);
        data[17] = (uint8)((tmp64 >> 8) & 0xFFu);
        data[18] = (uint8)((tmp64 >> 16) & 0xFFu);
        data[19] = (uint8)((tmp64 >> 24) & 0xFFu);
        data[20] = (uint8)((tmp64 >> 32) & 0xFFu);
        data[21] = (uint8)((tmp64 >> 40) & 0xFFu);
        data[22] = (uint8)((tmp64 >> 48) & 0xFFu);
        data[23] = (uint8)((tmp64 >> 56) & 0xFFu);
        (void) memcpy((void*)&tmp64, (const void*)&block->b1d, sizeof(uint64));
        data[24] = (uint8)(tmp64 & 0xFFu);
        data[25] = (uint8)((tmp64 >> 8) & 0xFFu);
        data[26] = (uint8)((tmp64 >> 16) & 0xFFu);
        data[27] = (uint8)((tmp64 >> 24) & 0xFFu);
        data[28] = (uint8)((tmp64 >> 32) & 0xFFu);
        data[29] = (uint8)((tmp64 >> 40) & 0xFFu);
        data[30] = (uint8)((tmp64 >> 48) & 0xFFu);
        data[31] = (uint8)((tmp64 >> 56) & 0xFFu);
        (void) memcpy((void*)&tmp64, (const void*)&block->a0d, sizeof(uint64));
        data[32] = (uint8)(tmp64 & 0xFFu);
        data[33] = (uint8)((tmp64 >> 8) & 0xFFu);
        data[34] = (uint8)((tmp64 >> 16) & 0xFFu);
        data[35] = (uint8)((tmp64 >> 24) & 0xFFu);
        data[36] = (uint8)((tmp64 >> 32) & 0xFFu);
        data[37] = (uint8)((tmp64 >> 40) & 0xFFu);
        data[38] = (uint8)((tmp64 >> 48) & 0xFFu);
        data[39] = (uint8)((tmp64 >> 56) & 0xFFu);
        *dataLength = 40u;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PIDLimit_Float64_Save(PIDLIMIT_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 40u)
    {
        error = 1u;
    }
    else
    {
        uint64 tmp64;
        tmp64 = (uint64)((uint64)data[0] | ((uint64)data[1] << 8) | ((uint64)data[2] << 16) | ((uint64)data[3] << 24) | 
            ((uint64)data[4] << 32) | ((uint64)data[5] << 40) | ((uint64)data[6] << 48) | ((uint64)data[7] << 56));
        (void) memcpy((void*)&block->b0, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[8] | ((uint64)data[9] << 8) | ((uint64)data[10] << 16) | ((uint64)data[11] << 24) | 
            ((uint64)data[12] << 32) | ((uint64)data[13] << 40) | ((uint64)data[14] << 48) | ((uint64)data[15] << 56));
        (void) memcpy((void*)&block->b1, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[16] | ((uint64)data[17] << 8) | ((uint64)data[18] << 16) | ((uint64)data[19] << 24) | 
            ((uint64)data[20] << 32) | ((uint64)data[21] << 40) | ((uint64)data[22] << 48) | ((uint64)data[23] << 56));
        (void) memcpy((void*)&block->b0d, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[24] | ((uint64)data[25] << 8) | ((uint64)data[26] << 16) | ((uint64)data[27] << 24) | 
            ((uint64)data[28] << 32) | ((uint64)data[29] << 40) | ((uint64)data[30] << 48) | ((uint64)data[31] << 56));
        (void) memcpy((void*)&block->b1d, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[32] | ((uint64)data[33] << 8) | ((uint64)data[34] << 16) | ((uint64)data[35] << 24) | 
            ((uint64)data[36] << 32) | ((uint64)data[37] << 40) | ((uint64)data[38] << 48) | ((uint64)data[39] << 56));
        (void) memcpy((void*)&block->a0d, (const void*)&tmp64, sizeof(float64));
        error = 0u;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PIDLIMIT_FLOAT64_ISLINKED)
void* PIDLimit_Float64_GetAddress(PIDLIMIT_FLOAT64* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = block->Init;
            break;
        case 3:
            addr = block->max;
            break;
        case 4:
            addr = block->min;
            break;
        case 5:
            addr = block->Enable;
            break;
        case 6:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
