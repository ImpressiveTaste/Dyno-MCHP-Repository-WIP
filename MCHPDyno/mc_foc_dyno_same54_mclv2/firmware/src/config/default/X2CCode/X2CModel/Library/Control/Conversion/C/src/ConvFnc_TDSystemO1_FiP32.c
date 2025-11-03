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
#include "ConvFnc_TDSystemO1_FiP32.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    int32 a11;
    int32 b11;
    int32 c11;
    int32 d11;
    uint8 sfra11;
    uint8 sfrb11;
    uint8 sfrc11;
    uint8 sfrd11;
} TDSYSTEMO1_FIP32_IMPL_PARAM;

/** Private prototypes */
static uint8 convertM2I(const TDSYSTEMO1_FIP32_MASK_PARAM *maskParam, TDSYSTEMO1_FIP32_IMPL_PARAM *impParam);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const TDSYSTEMO1_FIP32_MASK_PARAM *maskParam, TDSYSTEMO1_FIP32_IMPL_PARAM *impParam)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
    float_CoT mp_a11, mp_b11, mp_c11, mp_d11;

    /* get mask parameter values */
    mp_a11 = (float_CoT)(maskParam->A);
    mp_b11 = (float_CoT)(maskParam->B);
    mp_c11 = (float_CoT)(maskParam->C);
    mp_d11 = (float_CoT)(maskParam->D);

    /* set shift factors and Q values */
    impParam->sfra11 = getQFormat(mp_a11, 32);
    impParam->sfrb11 = getQFormat(mp_b11, 32);
    impParam->sfrc11 = getQFormat(mp_c11, 32);
    impParam->sfrd11 = getQFormat(mp_d11, 32);
    impParam->a11 = getQx32Value(mp_a11, (impParam->sfra11));
    impParam->b11 = getQx32Value(mp_b11, (impParam->sfrb11));
    impParam->c11 = getQx32Value(mp_c11, (impParam->sfrc11));
    impParam->d11 = getQx32Value(mp_d11, (impParam->sfrd11));

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
uint8 TDSystemO1_FiP32_LoadMP(const TDSYSTEMO1_FIP32_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (32U > maxSize)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((*(uint64*)&(maskParam->A)) & 0x00000000000000FF);
        data[1] = (uint8)((*(uint64*)&(maskParam->A) >> 8) & 0x00000000000000FF);
        data[2] = (uint8)((*(uint64*)&(maskParam->A) >> 16) & 0x00000000000000FF);
        data[3] = (uint8)((*(uint64*)&(maskParam->A) >> 24) & 0x00000000000000FF);
        data[4] = (uint8)((*(uint64*)&(maskParam->A) >> 32) & 0x00000000000000FF);
        data[5] = (uint8)((*(uint64*)&(maskParam->A) >> 40) & 0x00000000000000FF);
        data[6] = (uint8)((*(uint64*)&(maskParam->A) >> 48) & 0x00000000000000FF);
        data[7] = (uint8)((*(uint64*)&(maskParam->A) >> 56) & 0x00000000000000FF);
        data[8] = (uint8)((*(uint64*)&(maskParam->B)) & 0x00000000000000FF);
        data[9] = (uint8)((*(uint64*)&(maskParam->B) >> 8) & 0x00000000000000FF);
        data[10] = (uint8)((*(uint64*)&(maskParam->B) >> 16) & 0x00000000000000FF);
        data[11] = (uint8)((*(uint64*)&(maskParam->B) >> 24) & 0x00000000000000FF);
        data[12] = (uint8)((*(uint64*)&(maskParam->B) >> 32) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(maskParam->B) >> 40) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(maskParam->B) >> 48) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(maskParam->B) >> 56) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(maskParam->C)) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(maskParam->C) >> 8) & 0x00000000000000FF);
        data[18] = (uint8)((*(uint64*)&(maskParam->C) >> 16) & 0x00000000000000FF);
        data[19] = (uint8)((*(uint64*)&(maskParam->C) >> 24) & 0x00000000000000FF);
        data[20] = (uint8)((*(uint64*)&(maskParam->C) >> 32) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(maskParam->C) >> 40) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(maskParam->C) >> 48) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(maskParam->C) >> 56) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(maskParam->D)) & 0x00000000000000FF);
        data[25] = (uint8)((*(uint64*)&(maskParam->D) >> 8) & 0x00000000000000FF);
        data[26] = (uint8)((*(uint64*)&(maskParam->D) >> 16) & 0x00000000000000FF);
        data[27] = (uint8)((*(uint64*)&(maskParam->D) >> 24) & 0x00000000000000FF);
        data[28] = (uint8)((*(uint64*)&(maskParam->D) >> 32) & 0x00000000000000FF);
        data[29] = (uint8)((*(uint64*)&(maskParam->D) >> 40) & 0x00000000000000FF);
        data[30] = (uint8)((*(uint64*)&(maskParam->D) >> 48) & 0x00000000000000FF);
        data[31] = (uint8)((*(uint64*)&(maskParam->D) >> 56) & 0x00000000000000FF);
        *dataLen = 32U;
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
uint8 TDSystemO1_FiP32_SaveMP(TDSYSTEMO1_FIP32 *block, TDSYSTEMO1_FIP32_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 32U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        TDSystemO1_FiP32_BackupMP(maskParam);

        /** - Save new mask parameters */
        tmp64 = (uint64)data[0] + \
            ((uint64)data[1] << 8) + ((uint64)data[2] << 16) + \
            ((uint64)data[3] << 24) + ((uint64)data[4] << 32) + \
            ((uint64)data[5] << 40) + ((uint64)data[6] << 48) + \
            ((uint64)data[7] << 56);
        maskParam->A = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[8] + \
            ((uint64)data[9] << 8) + ((uint64)data[10] << 16) + \
            ((uint64)data[11] << 24) + ((uint64)data[12] << 32) + \
            ((uint64)data[13] << 40) + ((uint64)data[14] << 48) + \
            ((uint64)data[15] << 56);
        maskParam->B = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[16] + \
            ((uint64)data[17] << 8) + ((uint64)data[18] << 16) + \
            ((uint64)data[19] << 24) + ((uint64)data[20] << 32) + \
            ((uint64)data[21] << 40) + ((uint64)data[22] << 48) + \
            ((uint64)data[23] << 56);
        maskParam->C = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[24] + \
            ((uint64)data[25] << 8) + ((uint64)data[26] << 16) + \
            ((uint64)data[27] << 24) + ((uint64)data[28] << 32) + \
            ((uint64)data[29] << 40) + ((uint64)data[30] << 48) + \
            ((uint64)data[31] << 56);
        maskParam->D = (float64)(*(float64*)&tmp64);

        error = TDSystemO1_FiP32_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            TDSystemO1_FiP32_RestoreMP(maskParam);
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
uint8 TDSystemO1_FiP32_InitMP(TDSYSTEMO1_FIP32 *block, const TDSYSTEMO1_FIP32_MASK_PARAM *maskParam)
{
    uint8 error = 0U;

    TDSYSTEMO1_FIP32_IMPL_PARAM *implParam = (TDSYSTEMO1_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->a11 & 0x000000FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->a11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[2] = (uint8)((implParam->a11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->a11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->b11 & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->b11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->b11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->b11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->c11 & 0x000000FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->c11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->c11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->c11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->d11 & 0x000000FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->d11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->d11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->d11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[16] = (uint8)implParam->sfra11;
    SaveFncDataBuffer[17] = (uint8)implParam->sfrb11;
    SaveFncDataBuffer[18] = (uint8)implParam->sfrc11;
    SaveFncDataBuffer[19] = (uint8)implParam->sfrd11;

    /** - Execute save function of block */
    error = TDSystemO1_FiP32_Save(block, SaveFncDataBuffer, 20);

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
uint8 TDSystemO1_FiP32_ConvertMP(TDSYSTEMO1_FIP32 *block, TDSYSTEMO1_FIP32_MASK_PARAM *maskParam)
{
    uint8 error;
    TDSYSTEMO1_FIP32_IMPL_PARAM *implParam = (TDSYSTEMO1_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->a11 & 0x000000FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->a11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[2] = (uint8)((implParam->a11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->a11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->b11 & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->b11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->b11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->b11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->c11 & 0x000000FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->c11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->c11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->c11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->d11 & 0x000000FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->d11 >> 8) & 0x000000FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->d11 >> 16) & 0x000000FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->d11 >> 24) & 0x000000FF);
    SaveFncDataBuffer[16] = (uint8)implParam->sfra11;
    SaveFncDataBuffer[17] = (uint8)implParam->sfrb11;
    SaveFncDataBuffer[18] = (uint8)implParam->sfrc11;
    SaveFncDataBuffer[19] = (uint8)implParam->sfrd11;

    /** - Execute save function of block */
    error = TDSystemO1_FiP32_Save(block, SaveFncDataBuffer, 20);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void TDSystemO1_FiP32_BackupMP(TDSYSTEMO1_FIP32_MASK_PARAM* maskParam)
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
void TDSystemO1_FiP32_RestoreMP(TDSYSTEMO1_FIP32_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
