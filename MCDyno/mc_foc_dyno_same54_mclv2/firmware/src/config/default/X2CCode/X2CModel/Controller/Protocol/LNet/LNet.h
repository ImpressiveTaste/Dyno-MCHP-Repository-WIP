/**
 * @file
 * @brief LNet protocol
 */
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
 * $LastChangedRevision: 3524 $
 * $LastChangedDate:: 2024-11-29 19:50:53 +0100#$
 */
#ifndef LNET_H
#define LNET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonFcts.h"

#define LNET_STD_SYN ((uint8)0x55)
#define LNET_OSM_SYN ((uint8)0x02)
/* can't be changed -> length dependency */
#define LNET_FILL    ((uint8)0x00)

/* state machine states */
typedef enum {
    LNET_STATE_SYN, LNET_STATE_NODE, LNET_STATE_SIZE_LSB, LNET_STATE_SIZE_MSB, LNET_STATE_DATA, LNET_STATE_FILL, LNET_STATE_CRC,
    LNET_STATE_SIZE_TOO_LARGE
} tLNetFrameState;

typedef struct tLNet tLNet;
struct tLNet {
    void (*communicate)(tProtocol* protocol);
    void (*enableSend)(tProtocol* protocol);

    uint16 frameSize;
    uint16 maxCommSize;
    uint8* frameData;

    void* serviceTable;

    tInterface* hwInterface;

    void (*setupRxTimeout)(tLNet* protocol, uint16 timeout);
    void (*updateRxTimeout)(tLNet* protocol, uint16 time);
    void (*resetRxTimeout)(tLNet* protocol);

    /* receive timeout */
    uint16 rxTimeout;
    /* receive timeout counter (from .rcvTimeout towards zero) */
    uint16 rxTimeoutCnt;

    /* slave node id */
    uint8 node;

    /* receive- & send state */
    tLNetFrameState rxState;
    tLNetFrameState txState;
    /* communication state (rx or tx) */
    tCommState commState;
    uint8 crc;
    /* framebuffer ptr to actual frame byte */
    uint16 frameBufferPtr;

    /* next state being cached in case of fill byte */
    tLNetFrameState nextState;
    /* last SYN type cached in case of fill byte */
    uint8 lastSyn;
};

/* Public functions */
void initLNet(tLNet* protocol, uint8* dataBuffer, uint16 dataBufferSize, uint8 node);

#ifdef __cplusplus
}
#endif

#endif
