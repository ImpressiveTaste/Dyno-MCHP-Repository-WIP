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
 * $LastChangedRevision: 3312 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: */
/* USERCODE-END:Description                                                                                           */
#ifndef USAVESIGNAL_BOOL_H
#define USAVESIGNAL_BOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonFcts.h"

#if !defined(USAVESIGNAL_BOOL_ISLINKED)
#define USAVESIGNAL_BOOL_ID ((uint16)351)

#if !defined(X2C_USE_UNION_FOR_POINTER)
typedef struct {
    uint16          ID;
    bool            *In;
} USAVESIGNAL_BOOL;
#else
typedef struct {
    uint16          ID;
    BOOL_PTR        In;
} USAVESIGNAL_BOOL;
#endif

#define USAVESIGNAL_BOOL_FUNCTIONS { \
    USAVESIGNAL_BOOL_ID, \
    (void (*)(void*))Common_Update, \
    (void (*)(void*))uSaveSignal_Bool_Init, \
    (tLoadImplementationParameter)uSaveSignal_Bool_Load, \
    (tSaveImplementationParameter)Common_Save, \
    (void* (*)(const void*, uint16))uSaveSignal_Bool_GetAddress }

/**********************************************************************************************************************/
/** Public prototypes                                                                                                **/
/**********************************************************************************************************************/
void uSaveSignal_Bool_Init(USAVESIGNAL_BOOL *pTuSaveSignal_Bool);
uint8 uSaveSignal_Bool_Load(const USAVESIGNAL_BOOL *pTuSaveSignal_Bool, uint8 data[], uint16 *dataLength, uint16 maxSize);
void* uSaveSignal_Bool_GetAddress(const USAVESIGNAL_BOOL *block, uint16 elementId);

#endif

#ifdef __cplusplus
}
#endif

#endif
