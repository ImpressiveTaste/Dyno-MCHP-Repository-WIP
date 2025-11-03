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
 * $LastChangedRevision: 3570 $
 * $LastChangedDate:: 2025-01-08 17:15:41 +0100#$
 */
#include <stddef.h>
#include "CommonFcts.h"

/** Pseudo Initialization */
void Common_Init(void* block)
{
    ;
}

/** Pseudo Update */
void Common_Update(void* block)
{
    ;
}

/** Pseudo Load block data */
uint8 Common_Load(const void* block, uint8 data[], uint16* dataLength, uint16 maxSize)
{
    *dataLength = 0U;
    return (0U);
}

/** Pseudo Save block data */
uint8 Common_Save(void* block, const uint8 data[], uint16 dataLength)
{
    uint8 retVal;
    if (dataLength > 0U)
    {
        retVal = 1U;
    }
    else
    {
        retVal = 0U;
    }
    return (retVal);
}

/** Pseudo Get Address */
void* Common_GetAddress(void* block, uint16 elementId)
{
    return (NULL);
}

/** Pseudo Init Mask Parameter */
uint8 Common_InitMP(void *block, const void *maskParam)
{
    return (0U);
}

/** Pseudo Save Mask Parameter */
uint8 Common_SaveMP(void* block, void* maskParam, const uint8 data[], uint16 dataLength)
{
    return (0U);
}

/** Pseudo Load Mask Parameter */
uint8 Common_LoadMP(const void* block, uint8 data[], uint16* dataLength, uint16 maxSize)
{
    *dataLength = 0U;
    return (0U);
}

/** Pseudo Mask Parameter Conversion */
uint8 Common_ConvertMP(void* block, void* maskParam)
{
    return (0U);
}

/** Pseudo Mask Parameter Backup */
void Common_BackupMP(void* maskParam)
{
    ;
}

/** Pseudo Mask Parameter Restore */
void Common_RestoreMP(void* maskParam)
{
    ;
}

/**
 * @brief Returns IO-Block-Parameter identifier table index for a given Parameter identifier.
 *
 * @param[in] ioParamTbl IO-Block-Parameter identifier table
 * @param[in] paramId Parameter identifier
 * @param[out] index Index
 *
 * @return Error code
 */
uint8 getIoParamIndex(const tIoParamIdEntry ioParamTbl[], uint16 paramId, uint16* index)
{
	uint16 i;
	uint8 error;

	if (ioParamTbl == NULL)
	{
		*index = 0;
		error = ERROR_TABLE_NOT_INIT;
	}
	else
	{
		i = 0;
		while ((paramId != ioParamTbl[i].id) && (ioParamTbl[i].id != 0U))
		{
			i++;
		}
		if (ioParamTbl[i].id == 0U)
		{
			*index = 0;
			error = ERROR_INVALID_ID;
		}
		else
		{
			*index = i;
			error = ERROR_SUCCESS;
		}
	}
	return (error);
}

/**
 * @brief Returns Block-Parameter identifier table index for a given Parameter identifier.
 *
 * @param[in] paramTable Block-Parameter identifier table
 * @param[in] paramId Parameter Identifier
 * @param[out] index Index
 *
 * @return Error code
 */
uint8 getBlockParamIndex(const tParameterTable* paramTable, uint16 paramId, uint16* index)
{
	uint16 i;
	uint8 error;

	/* send parameter ID error if no parameter table has been initialized */
	if (paramTable == NULL)
	{
		*index = 0;
		error = ERROR_TABLE_NOT_INIT;
	}
	else
	{
		i = 0;
		while((paramId != paramTable[i].uiParID) && (paramTable[i].uiParID != 0U))
		{
			i++;
		}
		/* check if parameter is located in parameter table */
		if (paramTable[i].uiParID == 0U)
		{
			*index = 0;
			error = ERROR_INVALID_ID;
		}
		else
		{
			*index = i;
			error = ERROR_SUCCESS;
		}
	}
	return (error);
}

/**
 * @brief Returns Mask Parameter identifier table index for a given Parameter identifier.
 *
 * @param[in] mpTable Mask Parameter identifier table
 * @param[in] paramId Parameter Identifier
 * @param[out] index Index
 *
 * @return Error code
 */
uint8 getMaskParamIndex(const tMaskParameterEntry* mpTable, uint16 paramId, uint16* index)
{
	uint16 i;
	uint8 error;

	/* send parameter ID error if no parameter table has been initialized */
	if (mpTable == NULL)
	{
		*index = 0;
		error = ERROR_TABLE_NOT_INIT;
	}
	else
	{
		i = 0;
		while((paramId != mpTable[i].id) && (mpTable[i].id != 0U))
		{
			i++;
		}
		/* check if parameter is located in parameter table */
		if (mpTable[i].id == 0U)
		{
			*index = 0;
			error = ERROR_INVALID_ID;
		}
		else
		{
			*index = i;
			error = ERROR_SUCCESS;
		}
	}
	return (error);
}

uint8 getMaskParamExtIndex(const tMaskParamExtRecord* mpTable, uint16 paramId, uint16* index)
{
    uint16 i;
    uint8 error;

    /* send parameter ID error if no parameter table has been initialized */
    if (mpTable == NULL)
    {
        *index = 0;
        error = ERROR_TABLE_NOT_INIT;
    }
    else
    {
        i = 0;
        while((paramId != mpTable[i].id) && (mpTable[i].id != 0U))
        {
            i++;
        }
        /* check if parameter is located in parameter table */
        if (mpTable[i].id == 0U)
        {
            *index = 0;
            error = ERROR_INVALID_ID;
        }
        else
        {
            *index = i;
            error = ERROR_SUCCESS;
        }
    }
    return (error);
}

/**
 * @brief Returns Block function table index for a given Block identifier.
 *
 * @param[in] fncTable Block function table
 * @param[in] blockId Block identifier
 * @param[out] index Index
 *
 * @return Error code
 */
uint8 getBlockFunctionIndex(const tBlockFunctions* fncTable, uint16 blockId, uint16* index)
{
	uint16 i;
	uint8 error;

	if (fncTable == NULL)
	{
		*index = 0;
		error = ERROR_TABLE_NOT_INIT;
	}
	else
	{
		i = 0;
		while ((blockId != fncTable[i].iBlockID) && (fncTable[i].iBlockID != 0U))
		{
			i++;
		}

		if (fncTable[i].iBlockID == 0U)
		{
			*index = 0;
			error = ERROR_INVALID_ID;
		}
		else
		{
			*index = i;
			error = ERROR_SUCCESS;
		}
	}
	return (error);
}

uint8 getMaskParamDataTableIndex(const tMaskParamDataRecord* mpTable, uint16 blockParamId, uint8 maskParamId, uint16* index)
{
    uint16 i = 0;
    /* returns error if table is not initialized */
    if (mpTable == NULL)
    {
        return 1;
    }
    /* Block Parameter ID and Mask Parameter ID must not be zero */
    if ((blockParamId == 0U) || (maskParamId == 0U))
    {
        return 1;
    }

    while (mpTable[i].blockParamId != 0U)
    {
        if ((mpTable[i].blockParamId == blockParamId) && (mpTable[i].maskParamId == maskParamId))
        {
            *index = i;
            return 0;
        }
        else
        {
            i++;
        }
    }

    /* returns error if no record was found */
    return 2;
}

/**
 * @brief Returns Q-format for given decimal value.
 *
 * @param   decValue Decimal value (-2147483647.0 ... +2147483647.0)
 * @param   maxBits Maximum bit width (8 ... 32)
 *
 * @return  Q-Format (0 ... 31)
 */
uint8 getQFormat(float_CoT decValue, uint8 maxBits)
{
    uint8 qFormat;
    uint32 maxValue;
    float_CoT realValue;

    /* check and limit input arguments if necessary */
    if (maxBits > 32U)
    {
        maxBits = 32;
    }
    else if (maxBits < 8U)
    {
        maxBits = 8;
    }
    else
    {
        /* else: Max bits value within limits, no correction required */
    }
    LIMIT(decValue, (float_CoT)INT32_MAX);

    /* take absolute value of decValue to simplify calculation */
    if (decValue < (float_CoT)0)
    {
        decValue = -decValue;
    }

    /* calculate qFormat */
    /* Note: float64 has to be used rather than float_CoT, because float32 causes losses in accuracy for FiP32 CoT implementations, which lets CUnit tests fail */
    qFormat = maxBits;
    maxValue = (((uint32)1) << (maxBits-1U)) - 1U;
    do {
        qFormat--;
        realValue = (float_CoT)(decValue * (float64)((uint32)1 << qFormat));
    } while ((qFormat > 0U) && (realValue > (float64)maxValue));

    return (qFormat);
}


/**
 * @brief Returns integer value in Qx.8-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 7)
 *
 * @return  Q-format integer value (-127 ... +127)
 */
int8 getQx8Value(float_CoT decValue, uint8 qFormat)
{
    int16 intValue;

    intValue = (int16)(decValue * (float_CoT)((uint8)1 << qFormat));
    LIMIT(intValue, INT8_MAX);

    return (int8)intValue;
}

/**
 * @brief Returns integer value in Qx.16-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 15)
 *
 * @return  Q-format integer value (-32767 ... +32767)
 */
int16 getQx16Value(float_CoT decValue, uint8 qFormat)
{
    int32 intValue;

    intValue = (int32)(decValue * (float_CoT)((uint16)1 << qFormat));
    LIMIT(intValue, INT16_MAX);

    return (int16)intValue;
}

/**
 * @brief Returns integer value in Qx.32-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 31)
 *
 * @return  Q-format integer value (-2147483647 ... +2147483647)
 */
int32 getQx32Value(float_CoT decValue, uint8 qFormat)
{
    int64 intValue;

    intValue = (int64)(decValue * (float_CoT)((uint32)1 << qFormat));
    LIMIT(intValue, INT32_MAX);

    return (int32)intValue;
}

/**
 * @brief Returns asymmetric integer value (for angular signals) in Qx.8-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 7)
 *
 * @return  Q-format integer value (-128 ... +127)
 */
int8 getAQx8Value(float_CoT decValue, uint8 qFormat)
{
    int16 intValue;

    intValue = (int16)(decValue * (float_CoT)((uint8)1 << qFormat));
    if (intValue > INT8_MAX)
    {
        intValue = INT8_MAX;
    }
    else if (intValue < INT8_MIN)
    {
        intValue = INT8_MIN;
    }

    return (int8)intValue;
}

/**
 * @brief Returns asymmetric integer value (for angular signals) in Qx.16-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 15)
 *
 * @return  Q-format integer value (-32768 ... +32767)
 */
int16 getAQx16Value(float_CoT decValue, uint8 qFormat)
{
    int32 intValue;

    intValue = (int32)(decValue * (float_CoT)((uint16)1 << qFormat));
    if (intValue > INT16_MAX)
    {
        intValue = INT16_MAX;
    }
    else if (intValue < INT16_MIN)
    {
        intValue = INT16_MIN;
    }

    return (int16)intValue;
}

/**
 * @brief Returns asymmetric integer value (for angular signals) in Qx.32-format for given decimal value.
 *
 * @param   decValue Decimal value (-1.0 ... +1.0)
 * @param   qFormat Q-format (0 ... 31)
 *
 * @return  Q-format integer value (-2147483648 ... +2147483647)
 */
int32 getAQx32Value(float_CoT decValue, uint8 qFormat)
{
    int64 intValue;

    intValue = (int64)(decValue * (float_CoT)((uint32)1 << qFormat));
    if (intValue > INT32_MAX)
    {
        intValue = INT32_MAX;
    }
    else if (intValue < INT32_MIN)
    {
        intValue = INT32_MIN;
    }

    return (int32)intValue;
}
