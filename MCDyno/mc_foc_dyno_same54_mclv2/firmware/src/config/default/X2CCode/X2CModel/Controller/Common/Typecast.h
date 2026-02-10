/**
 * @file
 * @brief Typecast functions.
 *
 * Provides functions to cast unsigned to signed datatypes with an memory data width greater than the data type.
 * For example: Cast uint8 to int8 on a target using memory with 16-bit data width
 */ 
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
 * $LastChangedRevision: 3482 $
 * $LastChangedDate:: 2016-02-18 12:04:02 +0100#$
 */
#ifndef TYPECAST_H
#define TYPECAST_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
static __inline int8 castUint8ToInt8(uint8 value)
#else
static inline int8 castUint8ToInt8(uint8 value)
#endif
{
    int8 castedValue;
#if defined(__DATA_WIDTH_16BIT__)
    if ((value & 0x80U) != 0U)
    {
        castedValue = (int8)(uint16)((uint16)0xFF00U | value);
    }
    else
    {
        castedValue = (int8)value;
    }
#elif defined(__DATA_WIDTH_8BIT__)
    castedValue = (int8)value;
#elif defined(__MATLAB__) || defined(__SCILAB__)
    castedValue = (int8)value;
#else
#error DATA WIDTH NOT DEFINED
#endif
    return (castedValue);
}

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
static __inline int16 castUint16ToInt16(uint16 value)
#else
static inline int16 castUint16ToInt16(uint16 value)
#endif
{
    int16 castedValue;
#if defined(__DATA_WIDTH_16BIT__)
    castedValue = (int16)value;
#elif defined(__DATA_WIDTH_8BIT__)
    castedValue = (int16)value;
#elif defined(__MATLAB__) || defined(__SCILAB__)
    castedValue = (int16)value;
#else
#error DATA WIDTH NOT DEFINED
#endif
    return (castedValue);
}

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
static __inline int32 castUint32ToInt32(uint32 value)
#else
static inline int32 castUint32ToInt32(uint32 value)
#endif
{
    int32 castedValue;
#if defined(__DATA_WIDTH_16BIT__)
    castedValue = (int32)value;
#elif defined(__DATA_WIDTH_8BIT__)
    castedValue = (int32)value;
#elif defined(__MATLAB__) || defined(__SCILAB__)
    castedValue = (int32)value;
#else
#error DATA WIDTH NOT DEFINED
#endif
    return (castedValue);
}

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
static __inline int64 castUint64ToInt64(uint64 value)
#else
static inline int64 castUint64ToInt64(uint64 value)
#endif
{
    int64 castedValue;
#if defined(__DATA_WIDTH_16BIT__)
    castedValue = (int64)value;
#elif defined(__DATA_WIDTH_8BIT__)
    castedValue = (int64)value;
#elif defined(__MATLAB__) || defined(__SCILAB__)
    castedValue = (int64)value;
#else
#error DATA WIDTH NOT DEFINED
#endif
    return (castedValue);
}

#if defined(__DATA_WIDTH_16BIT__)
/** Deprecated - Use castUint8ToInt8 instead */
#define UINT8_TO_INT8(data)   ((int8)(((int16)(data) << 8) >> 8))
/** Deprecated - Use castUint16ToInt16 instead */
#define UINT16_TO_INT16(data) ((int16)(data))
/** Deprecated - Use castUint32ToInt32 instead */
#define UINT32_TO_INT32(data) ((int32)(data))
/** Deprecated - Use castUint64ToInt64 instead */
#define UINT64_TO_INT64(data) ((int64)(data))
#elif defined(__DATA_WIDTH_8BIT__)
/** Deprecated - Use castUint8ToInt8 instead */
#define UINT8_TO_INT8(data)   ((int8)(data))
/** Deprecated - Use castUint16ToInt16 instead */
#define UINT16_TO_INT16(data) ((int16)(data))
/** Deprecated - Use castUint32ToInt32 instead */
#define UINT32_TO_INT32(data) ((int32)(data))
/** Deprecated - Use castUint64ToInt64 instead */
#define UINT64_TO_INT64(data) ((int64)(data))
#elif defined(__MATLAB__) || defined(__SCILAB__)
/** Deprecated - Use castUint8ToInt8 instead */
#define UINT8_TO_INT8(data)   ((int8)(data))
/** Deprecated - Use castUint16ToInt16 instead */
#define UINT16_TO_INT16(data) ((int16)(data))
/** Deprecated - Use castUint32ToInt32 instead */
#define UINT32_TO_INT32(data) ((int32)(data))
/** Deprecated - Use castUint64ToInt64 instead */
#define UINT64_TO_INT64(data) ((int64)(data))
#else
#error DATA WIDTH NOT DEFINED
#endif

#ifdef __cplusplus
}
#endif

#endif
