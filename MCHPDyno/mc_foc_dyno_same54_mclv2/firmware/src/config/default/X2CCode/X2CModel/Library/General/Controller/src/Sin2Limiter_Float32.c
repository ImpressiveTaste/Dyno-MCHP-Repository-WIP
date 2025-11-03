/*
 * Copyright (c) 2018, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
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
/**     Description:    Continuously differentiable output change rate limitation with  sin^2 shape                  **/
/**                     Calculation:                                                                                 **/
/**                         du/dt = 0:                                                                               **/
/**                             y = u                                                                                **/
/**                         du/dt <> 0:                                                                              **/
/**                             y = u * sin^2(t)        (simplified)                                                 **/
/**                                                                                                                  **/
/**     Caution:    A step in the input signal during a running limitation process will not abort the running        **/
/**                 limitation process!                                                                              **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Sin2Limiter_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Sin2Limiter_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include <math.h>

/* Inputs */
#define IN                  (*block->In)
#define INIT                (*block->Init)
#define ENABLE              (*block->Enable)

/* Outputs */
#define OUT                 (block->Out)

/* Parameters */
#define RATE_UP             (block->RateUp)             /* rising slew rate */
#define RATE_DOWN           (block->RateDown)           /* falling slew rate */

/* Variables */
#define SCALED_RATE_UP      (block->Scaled_RateUp)      /* to step height adjusted rising slew rate */
#define SCALED_RATE_DOWN    (block->Scaled_RateDown)    /* to step height adjusted falling slew rate */
#define OUT_END             (block->Out_end)            /* desired target value */
#define LEVEL               (block->Level)              /* current input level (argument) for sin^2 calculation */
#define STEP_HEIGHT         (block->Step_Height)        /* current step height */
#define STATE               (block->State)              /* 1 -> limiting at rising flank, -1 -> limiting at falling flank, 0 -> no limiting */
#define ENABLE_OLD          (block->Enable_old)

/* Constants */
#define HALFPI              (PI_R32/(float32)2.0)                       /* pi/2 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Sin2Limiter_Float32_Update(SIN2LIMITER_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 sin2;
    float32 derivative, temp;

    if (!ENABLE)    /* Rate limiting disabled */
    {
        OUT = 0;
    }
    else            /* Rate limiting enabled */
    {
        if (!ENABLE_OLD)
        {
            /* rising edge of enable signal occurred */
            OUT = INIT; /* assign output to init value */
            STATE = 0;  /* reset state */
        }

        if (STATE == 0)
        {
            /* derivative */
            derivative = IN - OUT;

            /* check for change in input signal */
            if (derivative > 0)         /* positive step */
            {
                STATE = 1;                      /* save direction information */
                LEVEL = HALFPI;                 /* preset level */
                STEP_HEIGHT = derivative;       /* save step height*/
                OUT_END = IN;

                /* justification of rate factor in respect to step height */
                SCALED_RATE_UP = RATE_UP / STEP_HEIGHT;
            }
            else
            {
                if (derivative < 0)     /* negative step */
                {
                    STATE = -1;                 /* save direction information */
                    LEVEL = HALFPI;             /* preset level */
                    STEP_HEIGHT = -derivative;  /* save step height*/
                    OUT_END = IN;

                    /* justification of rate factor in respect to step height */
                    SCALED_RATE_DOWN = RATE_DOWN / STEP_HEIGHT;
                }
            }
        }

        switch(STATE)
        {
            case 0:     /* no rate limiting in progress */
                OUT = IN;
                break;

            case 1:     /* positive rate limiting */
                if (LEVEL <= SCALED_RATE_UP)   /* check if level has reached final value */
                {
                    /* rate limitation has finished */
                    LEVEL = 0;
                    STATE = 0;
                }
                else
                {
                    LEVEL -= SCALED_RATE_UP;    /* calculate new level value */
                }
                sin2 = sin(LEVEL);              /* calculate sin(level) */
                sin2 = sin2 * sin2;             /* calculate sin^2(level) */
                temp = sin2 * STEP_HEIGHT;      /* scale sin^2 to step height */
                OUT = OUT_END - temp;           /* subtract sin^2 from desired end value */

                break;

            case -1:    /* negative rate limiting */
                if (LEVEL <= SCALED_RATE_DOWN)  /* check if level has reached final value */
                {
                    /* rate limitation has finished */
                    LEVEL = 0;
                    STATE = 0;
                }
                else
                {
                    LEVEL -= SCALED_RATE_DOWN;  /* calculate new level value */
                }
                sin2 = sin(LEVEL);              /* calculate sin(level) */
                sin2 = sin2 * sin2;             /* calculate sin^2(level) */
                sin2 = -sin2;                   /* adapt sin^2 to falling flank */
                temp = sin2 * STEP_HEIGHT;      /* scale sin^2 to step height */
                OUT  = OUT_END - temp;          /* subtract sin^2 from desired end value */
                break;
            default:    /* should not occur */
                OUT = IN;
                STATE = 0;
                break;
        }
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Sin2Limiter_Float32_Init(SIN2LIMITER_FLOAT32 *block)
{
    block->ID = SIN2LIMITER_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    SCALED_RATE_UP   = 0;
    SCALED_RATE_DOWN = 0;
    OUT_END = 0;
    LEVEL = 0;
    STEP_HEIGHT = 0;
    STATE = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin2Limiter_Float32_Load(const SIN2LIMITER_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 8U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        (void) memcpy((void*)&tmp32, (const void*)&block->RateUp, sizeof(uint32));
        data[0] = (uint8)(tmp32 & 0xFFU);
        data[1] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->RateDown, sizeof(uint32));
        data[4] = (uint8)(tmp32 & 0xFFU);
        data[5] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[6] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[7] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 8U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sin2Limiter_Float32_Save(SIN2LIMITER_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 8U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        tmp32 = (uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24));
        (void) memcpy((void*)&block->RateUp, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24));
        (void) memcpy((void*)&block->RateDown, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SIN2LIMITER_FLOAT32_ISLINKED)
void* Sin2Limiter_Float32_GetAddress(SIN2LIMITER_FLOAT32* block, uint16 elementId)
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
            addr = block->Enable;
            break;
        case 4:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
