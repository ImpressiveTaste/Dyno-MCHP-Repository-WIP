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
#include "PIDLimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PIDLimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)         /* Q31 */
#define ENABLE          (*block->Enable)     /* Q0 */
#define INIT            (*block->Init)       /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)        /* Q62 */
    #define LIMIT_DOWN  (*block->min)        /* Q62 */
#endif

/* Outputs */
#define OUT             (block->Out)         /* Q31 */

/* Parameter */
#define B0              (block->b0)          /* Qx */
#define B1              (block->b1)          /* Qy */
#define B0D             (block->b0d)         /* Qz */
#define B1D             (block->b1d)         /* Qz */
#define A0D             (block->a0d)         /* Q31 */

#define SFR0            (block->sfrb0)       /* x */
#define SFR1            (block->sfrb1)       /* y */
#define SFRD            (block->sfrd)        /* z */

/* Variables */
#define I_OLD           (block->i_old)       /* Q62 */
#define IN_OLD          (block->in_old)      /* Q31 */
#define D_OLD           (block->d_old)       /* Q31 */
#define ENABLE_OLD      (block->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PIDLimit_FiP32_Update(PIDLIMIT_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y, yp, yd;
    bool clamp;             /* Flag used for anti windup strategy clamping */


    if (ENABLE) /* Block enabled */
    {
        /* Proportional term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        yp = ((int64)B1 * (int64)IN) >> SFR1;           /* Q31 */

        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialization of previous values */
            IN_OLD = 0;
            D_OLD = 0;

            /* Calculation of initial derivative term */
            /* shorter calculation, since other differential terms equal zero */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
            yd = ((int64)B1D * (int64)IN) >> SFRD;      /* Q31 */

            /* Check if intial output is out of limits */
            if (INIT > LIMIT_UP)
            {
                y = (int64)LIMIT_UP;
            }
            else if (INIT < LIMIT_DOWN)
            {
                y = (int64)LIMIT_DOWN;
            }
            else
            {
                y = (int64)INIT;
            }

            /* Initialization of integral part so that first y = INIT */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            I_OLD = (y - yp - yd) << 31;
        }
        else
        {
            /* Calculation of derivative term */
            yd  =  (int64)B0D * (int64)IN_OLD;              /* Qz */
            yd += ((int64)B1D * (int64)IN);                 /* Qz */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
            yd  = yd >> SFRD;                               /* Qz -> Q31 */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
            yd -= (((int64)A0D * (int64)D_OLD) >> 31);      /* Q31 */
            
            /* Data width limitation Previous: LIMIT(yd, INT32_MAX); */
            if (yd > INT32_MAX)
            {
                yd = INT32_MAX;
            }
            else
            {
                if (yd < -INT32_MAX)
                {
                    yd = -INT32_MAX;
                }
            }
        }
        
        /* Sum */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y = yp + yd + (I_OLD >> 31);

        /* Output limitation and anti windup clamping */
        if (y > LIMIT_UP)
        {
            /* Output beyond upper limit */
            y = LIMIT_UP;

            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN > 0);
        }
        else if (y < LIMIT_DOWN)
        {
            /* Output beyond lower limit */
            y = LIMIT_DOWN;

            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN < 0);
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
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0003", true_no_defect) */
            I_OLD += (((int64)B0 * (int64)IN) << (31-SFR0));/* Q62 */
        }

        /* Assign output */
        OUT = (int32)y;

        /* Save relevant data for next call */
        IN_OLD = IN;
        D_OLD  = (int32)yd;
    }
    else            /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PIDLimit_FiP32_Init(PIDLIMIT_FIP32 *block)
{
    block->ID = PIDLIMIT_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    I_OLD   = 0;
    IN_OLD  = 0;
    D_OLD   = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PIDLimit_FiP32_Load(const PIDLIMIT_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 23u)
    {
        error = 1u;
    }
    else
    {
        data[0] = (uint8)((uint32)block->b0 & 0xFFu);
        data[1] = (uint8)(((uint32)block->b0 >> 8) & 0xFFu);
        data[2] = (uint8)(((uint32)block->b0 >> 16) & 0xFFu);
        data[3] = (uint8)(((uint32)block->b0 >> 24) & 0xFFu);
        data[4] = (uint8)((uint32)block->b1 & 0xFFu);
        data[5] = (uint8)(((uint32)block->b1 >> 8) & 0xFFu);
        data[6] = (uint8)(((uint32)block->b1 >> 16) & 0xFFu);
        data[7] = (uint8)(((uint32)block->b1 >> 24) & 0xFFu);
        data[8] = (uint8)((uint32)block->b0d & 0xFFu);
        data[9] = (uint8)(((uint32)block->b0d >> 8) & 0xFFu);
        data[10] = (uint8)(((uint32)block->b0d >> 16) & 0xFFu);
        data[11] = (uint8)(((uint32)block->b0d >> 24) & 0xFFu);
        data[12] = (uint8)((uint32)block->b1d & 0xFFu);
        data[13] = (uint8)(((uint32)block->b1d >> 8) & 0xFFu);
        data[14] = (uint8)(((uint32)block->b1d >> 16) & 0xFFu);
        data[15] = (uint8)(((uint32)block->b1d >> 24) & 0xFFu);
        data[16] = (uint8)((uint32)block->a0d & 0xFFu);
        data[17] = (uint8)(((uint32)block->a0d >> 8) & 0xFFu);
        data[18] = (uint8)(((uint32)block->a0d >> 16) & 0xFFu);
        data[19] = (uint8)(((uint32)block->a0d >> 24) & 0xFFu);
        data[20] = (uint8)(block->sfrb0 & 0xFFu);
        data[21] = (uint8)(block->sfrb1 & 0xFFu);
        data[22] = (uint8)(block->sfrd & 0xFFu);
        *dataLength = 23u;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PIDLimit_FiP32_Save(PIDLIMIT_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 23u)
    {
        error = 1u;
    }
    else
    {
        block->b0 = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->b1 = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->b0d = castUint32ToInt32((uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24)));
        block->b1d = castUint32ToInt32((uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24)));
        block->a0d = castUint32ToInt32((uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24)));
        block->sfrb0 = data[20];
        block->sfrb1 = data[21];
        block->sfrd = data[22];
        error = 0u;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PIDLIMIT_FIP32_ISLINKED)
void* PIDLimit_FiP32_GetAddress(PIDLIMIT_FIP32* block, uint16 elementId)
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
