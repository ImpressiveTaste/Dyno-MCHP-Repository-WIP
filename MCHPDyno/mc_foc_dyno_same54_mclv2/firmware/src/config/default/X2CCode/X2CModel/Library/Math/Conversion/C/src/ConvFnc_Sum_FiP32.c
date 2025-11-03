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
/* Description: */
/* USERCODE-END:Description                                                                                           */
#include <string.h>
#include "ConvFnc_Sum_FiP32.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    uint16 sign;
} SUM_FIP32_IMPL_PARAM;

/** In1 enumeration */
typedef enum {
    IN1_P = 0,
    IN1_M = 1,
    IN1_0 = 2
} tIn1;

/** In2 enumeration */
typedef enum {
    IN2_P = 0,
    IN2_M = 1,
    IN2_0 = 2
} tIn2;

/** In3 enumeration */
typedef enum {
    IN3_P = 0,
    IN3_M = 1,
    IN3_0 = 2
} tIn3;

/** In4 enumeration */
typedef enum {
    IN4_P = 0,
    IN4_M = 1,
    IN4_0 = 2
} tIn4;

/** In5 enumeration */
typedef enum {
    IN5_P = 0,
    IN5_M = 1,
    IN5_0 = 2
} tIn5;

/** In6 enumeration */
typedef enum {
    IN6_P = 0,
    IN6_M = 1,
    IN6_0 = 2
} tIn6;

/** In7 enumeration */
typedef enum {
    IN7_P = 0,
    IN7_M = 1,
    IN7_0 = 2
} tIn7;

/** In8 enumeration */
typedef enum {
    IN8_P = 0,
    IN8_M = 1,
    IN8_0 = 2
} tIn8;

/** Private prototypes */
static uint8 convertM2I(const SUM_FIP32_MASK_PARAM *maskParam, SUM_FIP32_IMPL_PARAM *impParam);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const SUM_FIP32_MASK_PARAM *maskParam, SUM_FIP32_IMPL_PARAM *impParam)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
    uint16 mask = 0;

    switch(maskParam->In1)
    {
        case IN1_P:
            mask = mask | 0x0001;
            break;
        case IN1_M:
            mask = mask | 0x0002;
            break;
        default:
            break;
    }
    switch(maskParam->In2)
    {
        case IN2_P:
            mask = mask | 0x0004;
            break;
        case IN2_M:
            mask = mask | 0x0008;
            break;
        default:
            break;
    }
    switch(maskParam->In3)
    {
        case IN3_P:
            mask = mask | 0x0010;
            break;
        case IN3_M:
            mask = mask | 0x0020;
            break;
        default:
            break;
    }
    switch(maskParam->In4)
    {
        case IN4_P:
            mask = mask | 0x0040;
            break;
        case IN4_M:
            mask = mask | 0x0080;
            break;
        default:
            break;
    }
    switch(maskParam->In5)
    {
        case IN5_P:
            mask = mask | 0x0100;
            break;
        case IN5_M:
            mask = mask | 0x0200;
            break;
        default:
            break;
    }
    switch(maskParam->In6)
    {
        case IN6_P:
            mask = mask | 0x0400;
            break;
        case IN6_M:
            mask = mask | 0x0800;
            break;
        default:
            break;
    }
    switch(maskParam->In7)
    {
        case IN7_P:
            mask = mask | 0x1000;
            break;
        case IN7_M:
            mask = mask | 0x2000;
            break;
        default:
            break;
    }
    switch(maskParam->In8)
    {
        case IN8_P:
            mask = mask | 0x4000;
            break;
        case IN8_M:
            mask = mask | 0x8000;
            break;
        default:
            break;
    }

    impParam->sign = mask;

/* USERCODE-END:Conversion                                                                                            */
    return (error);
}

/**
 * @brief Load block mask parameter data.
 *
 * @param[in] maskParam Mask parameter data structure
 * @param[out] data Data
 * @param[out] dataLen Data length
 * @param[in] maxSize Maximum Service data length
 *
 * @return Error: zero on success, not zero in case of load- or conversion error
 */
uint8 Sum_FiP32_LoadMP(const SUM_FIP32_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (8U > maxSize)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(maskParam->In1);
        data[1] = (uint8)(maskParam->In2);
        data[2] = (uint8)(maskParam->In3);
        data[3] = (uint8)(maskParam->In4);
        data[4] = (uint8)(maskParam->In5);
        data[5] = (uint8)(maskParam->In6);
        data[6] = (uint8)(maskParam->In7);
        data[7] = (uint8)(maskParam->In8);
        *dataLen = 8U;
    }
    return (error);
}

/**
 * @brief Save block mask parameter data.
 *
 * @param[in] block Pointer to block structure
 * @param[out] maskParam Mask parameter data structure
 * @param[in] data Data
 * @param dataLen Length of mask parameter data stream
 *
 * @return Error: zero on success, not zero in case of save- or conversion error
 */
uint8 Sum_FiP32_SaveMP(SUM_FIP32 *block, SUM_FIP32_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 8U)
    {
        error = 1U;
    }
    else
    {
        /** - Cache old mask parameters */
        Sum_FiP32_BackupMP(maskParam);

        /** - Save new mask parameters */
        maskParam->In1 = data[0];
        maskParam->In2 = data[1];
        maskParam->In3 = data[2];
        maskParam->In4 = data[3];
        maskParam->In5 = data[4];
        maskParam->In6 = data[5];
        maskParam->In7 = data[6];
        maskParam->In8 = data[7];

        error = Sum_FiP32_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            Sum_FiP32_RestoreMP(maskParam);
        }

    }
    return (error);
}

/**
 * @brief Initializes Implementation parameters from Mask parameters.
 *
 * @param[out] block Block
 * @param[in] maskParam Mask parameters
 *
 * @return Error: zero on success, not zero in case of initialization error
 */
uint8 Sum_FiP32_InitMP(SUM_FIP32 *block, const SUM_FIP32_MASK_PARAM *maskParam)
{
    uint8 error = 0U;

    SUM_FIP32_IMPL_PARAM *implParam = (SUM_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->sign & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->sign >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = Sum_FiP32_Save(block, SaveFncDataBuffer, 2);

    return (error);
}

/**
 * @brief Converts Mask parameters into Implementation parameters and executes Block Save function.
 *
 * @param[out] block Block Implementation
 * @param[in] maskParam Mask Parameters
 *
 * @return 0 if successful, greater than 0 if conversion failed
 */
uint8 Sum_FiP32_ConvertMP(SUM_FIP32 *block, SUM_FIP32_MASK_PARAM *maskParam)
{
    uint8 error;
    SUM_FIP32_IMPL_PARAM *implParam = (SUM_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->sign & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->sign >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = Sum_FiP32_Save(block, SaveFncDataBuffer, 2);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void Sum_FiP32_BackupMP(SUM_FIP32_MASK_PARAM* maskParam)
{
    memcpy(MaskParamBuffer, maskParam, sizeof(*maskParam));
}

/**
 * @brief Restores Mask parameters from global Mask parameter backup array.
 *
 * @param[out] maskParam Mask parameters
 *
 * @return Nothing
 */
void Sum_FiP32_RestoreMP(SUM_FIP32_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
