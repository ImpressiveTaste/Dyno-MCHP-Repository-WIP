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
#include "ConvFnc_LookupTable1D_Float64.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    uint16 dimX;
    float32 gainX;
    float64 offsetX;
} LOOKUPTABLE1D_FLOAT64_IMPL_PARAM;

/** DimX enumeration */
typedef enum {
    DIMX_17 = 0,
    DIMX_33 = 1,
    DIMX_65 = 2,
    DIMX_129 = 3,
    DIMX_257 = 4,
    DIMX_513 = 5,
    DIMX_1025 = 6,
    DIMX_2049 = 7,
    DIMX_4097 = 8
} tDimX;

/** Private prototypes */
static uint8 convertM2I(const LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam, LOOKUPTABLE1D_FLOAT64_IMPL_PARAM *impParam);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam, LOOKUPTABLE1D_FLOAT64_IMPL_PARAM *impParam)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
    error = 1;  /* conversion of flash table is not supported/reasonable */

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
uint8 LookupTable1D_Float64_LoadMP(const LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (25U > maxSize)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((*(uint64*)&(maskParam->TableData)) & 0x00000000000000FF);
        data[1] = (uint8)((*(uint64*)&(maskParam->TableData) >> 8) & 0x00000000000000FF);
        data[2] = (uint8)((*(uint64*)&(maskParam->TableData) >> 16) & 0x00000000000000FF);
        data[3] = (uint8)((*(uint64*)&(maskParam->TableData) >> 24) & 0x00000000000000FF);
        data[4] = (uint8)((*(uint64*)&(maskParam->TableData) >> 32) & 0x00000000000000FF);
        data[5] = (uint8)((*(uint64*)&(maskParam->TableData) >> 40) & 0x00000000000000FF);
        data[6] = (uint8)((*(uint64*)&(maskParam->TableData) >> 48) & 0x00000000000000FF);
        data[7] = (uint8)((*(uint64*)&(maskParam->TableData) >> 56) & 0x00000000000000FF);
        data[8] = (uint8)(maskParam->DimX);
        data[9] = (uint8)((*(uint64*)&(maskParam->x_min)) & 0x00000000000000FF);
        data[10] = (uint8)((*(uint64*)&(maskParam->x_min) >> 8) & 0x00000000000000FF);
        data[11] = (uint8)((*(uint64*)&(maskParam->x_min) >> 16) & 0x00000000000000FF);
        data[12] = (uint8)((*(uint64*)&(maskParam->x_min) >> 24) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(maskParam->x_min) >> 32) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(maskParam->x_min) >> 40) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(maskParam->x_min) >> 48) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(maskParam->x_min) >> 56) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(maskParam->x_max)) & 0x00000000000000FF);
        data[18] = (uint8)((*(uint64*)&(maskParam->x_max) >> 8) & 0x00000000000000FF);
        data[19] = (uint8)((*(uint64*)&(maskParam->x_max) >> 16) & 0x00000000000000FF);
        data[20] = (uint8)((*(uint64*)&(maskParam->x_max) >> 24) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(maskParam->x_max) >> 32) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(maskParam->x_max) >> 40) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(maskParam->x_max) >> 48) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(maskParam->x_max) >> 56) & 0x00000000000000FF);
        *dataLen = 25U;
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
uint8 LookupTable1D_Float64_SaveMP(LOOKUPTABLE1D_FLOAT64 *block, LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 25U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        LookupTable1D_Float64_BackupMP(maskParam);

        /** - Save new mask parameters */
        tmp64 = (uint64)data[0] + \
            ((uint64)data[1] << 8) + ((uint64)data[2] << 16) + \
            ((uint64)data[3] << 24) + ((uint64)data[4] << 32) + \
            ((uint64)data[5] << 40) + ((uint64)data[6] << 48) + \
            ((uint64)data[7] << 56);
        maskParam->TableData = (float64)(*(float64*)&tmp64);
        maskParam->DimX = data[8];
        tmp64 = (uint64)data[9] + \
            ((uint64)data[10] << 8) + ((uint64)data[11] << 16) + \
            ((uint64)data[12] << 24) + ((uint64)data[13] << 32) + \
            ((uint64)data[14] << 40) + ((uint64)data[15] << 48) + \
            ((uint64)data[16] << 56);
        maskParam->x_min = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[17] + \
            ((uint64)data[18] << 8) + ((uint64)data[19] << 16) + \
            ((uint64)data[20] << 24) + ((uint64)data[21] << 32) + \
            ((uint64)data[22] << 40) + ((uint64)data[23] << 48) + \
            ((uint64)data[24] << 56);
        maskParam->x_max = (float64)(*(float64*)&tmp64);

        error = LookupTable1D_Float64_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            LookupTable1D_Float64_RestoreMP(maskParam);
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
uint8 LookupTable1D_Float64_InitMP(LOOKUPTABLE1D_FLOAT64 *block, const LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam)
{
    uint8 error = 0U;

    LOOKUPTABLE1D_FLOAT64_IMPL_PARAM *implParam = (LOOKUPTABLE1D_FLOAT64_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->dimX & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->dimX >> 8) & 0x00FF);
    SaveFncDataBuffer[2] = (uint8)((*(uint32*)&(implParam->gainX)) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((*(uint32*)&(implParam->gainX) >> 8) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)((*(uint32*)&(implParam->gainX) >> 16) & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((*(uint32*)&(implParam->gainX) >> 24) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((*(uint64*)&(implParam->offsetX)) & 0x00000000000000FF);
    SaveFncDataBuffer[7] = (uint8)((*(uint64*)&(implParam->offsetX) >> 8) & 0x00000000000000FF);
    SaveFncDataBuffer[8] = (uint8)((*(uint64*)&(implParam->offsetX) >> 16) & 0x00000000000000FF);
    SaveFncDataBuffer[9] = (uint8)((*(uint64*)&(implParam->offsetX) >> 24) & 0x00000000000000FF);
    SaveFncDataBuffer[10] = (uint8)((*(uint64*)&(implParam->offsetX) >> 32) & 0x00000000000000FF);
    SaveFncDataBuffer[11] = (uint8)((*(uint64*)&(implParam->offsetX) >> 40) & 0x00000000000000FF);
    SaveFncDataBuffer[12] = (uint8)((*(uint64*)&(implParam->offsetX) >> 48) & 0x00000000000000FF);
    SaveFncDataBuffer[13] = (uint8)((*(uint64*)&(implParam->offsetX) >> 56) & 0x00000000000000FF);

    /** - Execute save function of block */
    error = LookupTable1D_Float64_Save(block, SaveFncDataBuffer, 14);

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
uint8 LookupTable1D_Float64_ConvertMP(LOOKUPTABLE1D_FLOAT64 *block, LOOKUPTABLE1D_FLOAT64_MASK_PARAM *maskParam)
{
    uint8 error;
    LOOKUPTABLE1D_FLOAT64_IMPL_PARAM *implParam = (LOOKUPTABLE1D_FLOAT64_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->dimX & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->dimX >> 8) & 0x00FF);
    SaveFncDataBuffer[2] = (uint8)((*(uint32*)&(implParam->gainX)) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((*(uint32*)&(implParam->gainX) >> 8) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)((*(uint32*)&(implParam->gainX) >> 16) & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((*(uint32*)&(implParam->gainX) >> 24) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((*(uint64*)&(implParam->offsetX)) & 0x00000000000000FF);
    SaveFncDataBuffer[7] = (uint8)((*(uint64*)&(implParam->offsetX) >> 8) & 0x00000000000000FF);
    SaveFncDataBuffer[8] = (uint8)((*(uint64*)&(implParam->offsetX) >> 16) & 0x00000000000000FF);
    SaveFncDataBuffer[9] = (uint8)((*(uint64*)&(implParam->offsetX) >> 24) & 0x00000000000000FF);
    SaveFncDataBuffer[10] = (uint8)((*(uint64*)&(implParam->offsetX) >> 32) & 0x00000000000000FF);
    SaveFncDataBuffer[11] = (uint8)((*(uint64*)&(implParam->offsetX) >> 40) & 0x00000000000000FF);
    SaveFncDataBuffer[12] = (uint8)((*(uint64*)&(implParam->offsetX) >> 48) & 0x00000000000000FF);
    SaveFncDataBuffer[13] = (uint8)((*(uint64*)&(implParam->offsetX) >> 56) & 0x00000000000000FF);

    /** - Execute save function of block */
    error = LookupTable1D_Float64_Save(block, SaveFncDataBuffer, 14);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void LookupTable1D_Float64_BackupMP(LOOKUPTABLE1D_FLOAT64_MASK_PARAM* maskParam)
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
void LookupTable1D_Float64_RestoreMP(LOOKUPTABLE1D_FLOAT64_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
