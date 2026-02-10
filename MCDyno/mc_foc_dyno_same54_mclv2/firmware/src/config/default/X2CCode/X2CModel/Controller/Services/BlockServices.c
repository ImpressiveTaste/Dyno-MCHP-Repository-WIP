/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM),  web: www.lcm.at
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
#include "Services.h"
#include "TableStruct.h"
#include "BlockServices.h"

/**
 * @brief Adds common block services to service table.
 *
 * @param[IN] protocol Protocol
 */
void addBlockServices(tProtocol* protocol)
{
    tSERVICEFunction* pServiceTable = (tSERVICEFunction*) protocol->pServiceTable;

    pServiceTable[SV_ID_SVGETRAMBLOCK] = &getRAMBlock;
    pServiceTable[SV_ID_GETBLOCKDATA] = &getBlockData;
    pServiceTable[SV_ID_PUTBLOCKDATA] = &putBlockData;
}

/**
 * @brief Adds extended block services to service table.
 *
 * @param[IN] protocol Protocol
 */
void addExtendedBlockServices(tProtocol* protocol)
{
	tSERVICEFunction* serviceTable = (tSERVICEFunction*) protocol->pServiceTable;
	serviceTable[SV_ID_SVPUTRAMBLOCK] = &putRAMBlock;
}

/**
 * @brief Writes data to target memory.
 *
 */
void putRAMBlock(tProtocol* protocol)
{
	void* addr;
	uint8 type;
	uint16 i, size;
	const uint8 addrOffset = (uint8)1;
#if defined(__ADDRESS_WIDTH_8BIT__)
	const uint8 typeOffset = addrOffset + (uint8)1;
#elif defined(__ADDRESS_WIDTH_16BIT__)
	const uint8 typeOffset = addrOffset + (uint8)2;
#elif defined(__ADDRESS_WIDTH_32BIT__)
	const uint8 typeOffset = addrOffset + (uint8)4;
#else
#error ADDRESS WIDTH NOT DEFINED
#endif
	const uint8 dataOffset = typeOffset + (uint8)1;

#if defined(__ADDRESS_WIDTH_8BIT__)
	addr = (void*)((uint8)protocol->ucFRAMEData[addrOffset]);
#elif defined(__ADDRESS_WIDTH_16BIT__)
	addr = (void*)((uint16)protocol->ucFRAMEData[addrOffset] + ((uint16)protocol->ucFRAMEData[addrOffset + 1] << 8));
#elif defined(__ADDRESS_WIDTH_32BIT__)
	addr = (void*)((uint32)protocol->ucFRAMEData[addrOffset] + ((uint32)protocol->ucFRAMEData[addrOffset + 1U] << 8) +
			((uint32)protocol->ucFRAMEData[addrOffset + 2U] << 16) + ((uint32)protocol->ucFRAMEData[addrOffset + 3U] << 24));
#else
#error ADDRESS WIDTH NOT DEFINED
#endif

	size = protocol->ucFRAMESize - dataOffset;
	type = protocol->ucFRAMEData[typeOffset];

	if ((size % type) != 0U)
	{
		sendError(protocol, ERRORFormat);
		return;
	}

	switch (type)
	{
		case DATATYPE_8BIT:
		{
			uint8* addr8 = (uint8*)addr;
			for (i=0; i<size; i++)
			{
				*addr8 = protocol->ucFRAMEData[dataOffset + i] & 0xFFU;
				addr8++;
			}
			break;
		}
		case DATATYPE_16BIT:
		{
			uint16* addr16 = (uint16*)addr;
			for (i=0; i<(size/2U); i++)
			{
				*addr16 = (uint16)protocol->ucFRAMEData[dataOffset + (2U*i)] + ((uint16)protocol->ucFRAMEData[dataOffset + (2U*i) + 1U] << 8);
				addr16++;
			}
			break;
		}
		case DATATYPE_32BIT:
		{
			uint32* addr32 = (uint32*)addr;
			for (i=0; i<(size/4U); i++)
			{
				*addr32 = (uint32)protocol->ucFRAMEData[dataOffset + (4U*i)] + ((uint32)protocol->ucFRAMEData[dataOffset + (4U*i) + 1U] << 8) +
						((uint32)protocol->ucFRAMEData[dataOffset + (4U*i) + 2U] << 16) + ((uint32)protocol->ucFRAMEData[dataOffset + (4U*i) + 3U] << 24);
				addr32++;
			}
			break;
		}
		case DATATYPE_64BIT:
		{
			uint64* addr64 = (uint64*)addr;
			for (i=0; i<(size/8U); i++)
			{
				*addr64 = (uint64)protocol->ucFRAMEData[dataOffset + (8U*i)] + ((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 1U] << 8) +
						((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 2U] << 16) + ((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 3U] << 24) +
						((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 4U] << 32) + ((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 5U] << 40) +
						((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 6U] << 48) + ((uint64)protocol->ucFRAMEData[dataOffset + (8U*i) + 7U] << 56);
				addr64++;
			}
			break;
		}
		default:
		{
			sendError(protocol, ERRORFormat);
			return;
		}
	}

	protocol->ucFRAMESize = (uint8)2;
	protocol->ucFRAMEData[1] = ERRORSuccess;
	protocol->pSnd_Enable(protocol);
}

/**
 * @brief Reads data from target memory.
 *        Service is protected by payload protection.
 *
 * @param[IN] protocol Protocol
 */
void getRAMBlock(tProtocol* protocol)
{
    void *addr;
    uint16 i, size;
    uint8 type;
    uint8 errorID = ERRORSuccess;
#if defined(__ADDRESS_WIDTH_8BIT__)
    const uint8 addrOffset = 2;
#elif defined(__ADDRESS_WIDTH_16BIT__)
    const uint8 addrOffset = 3;
#elif defined(__ADDRESS_WIDTH_32BIT__)
    const uint8 addrOffset = 5;
#else
#error ADDRESS WIDTH NOT DEFINED
#endif

    size = protocol->ucFRAMEData[addrOffset];
    size += (uint16)protocol->ucFRAMEData[addrOffset + 1U] << 8;
    type = protocol->ucFRAMEData[addrOffset + 2U];

    if ((size + 2U) > protocol->ucMaxCommSize)
    {
        errorID = ERRORSizeTooLarge;
    }
    else
    {
#if defined(__ADDRESS_WIDTH_8BIT__)
    	addr = (void*) ((uint8) protocol->ucFRAMEData[1];
#elif defined(__ADDRESS_WIDTH_16BIT__)
        addr = (void*) ((uint16) protocol->ucFRAMEData[1] + ((uint16) protocol->ucFRAMEData[2] << 8));
#elif defined(__ADDRESS_WIDTH_32BIT__)
        addr = (void*) ((uint32) protocol->ucFRAMEData[1] + ((uint32) protocol->ucFRAMEData[2] << 8) +
                ((uint32) protocol->ucFRAMEData[3] << 16) + ((uint32) protocol->ucFRAMEData[4] << 24));
#else
#error ADDRESS WIDTH NOT DEFINED
#endif

        switch (type)
        {
            case DATATYPE_8BIT:
            {
                uint8* addr8 = (uint8*) addr;
                for (i = 0; i < size; i++)
                {
                    protocol->ucFRAMEData[2U + i] = *addr8;
                    addr8++;
                }
                break;
            }
            case DATATYPE_16BIT:
            {
                uint16* addr16 = (uint16*) addr;
                for (i = 0; i < size; i += 2U)
                {
                    protocol->ucFRAMEData[2U + i] = (uint8) *addr16;
                    protocol->ucFRAMEData[2U + i + 1U] = (uint8) (*addr16 >> 8);
                    addr16++;
                }
                break;
            }
            case DATATYPE_32BIT:
            {
                uint32* addr32 = (uint32*) addr;
                for (i = 0; i < size; i += 4U)
                {
                    protocol->ucFRAMEData[2U + i] = (uint8) *addr32;
                    protocol->ucFRAMEData[2U + i + 1U] = (uint8) (*addr32 >> 8);
                    protocol->ucFRAMEData[2U + i + 2U] = (uint8) (*addr32 >> 16);
                    protocol->ucFRAMEData[2U + i + 3U] = (uint8) (*addr32 >> 24);
                    addr32++;
                }
                break;
            }
            case DATATYPE_64BIT:
            {
            	uint64* addr64 = (uint64*)addr;
            	for (i = 0; i < size; i += 8U)
            	{
                    protocol->ucFRAMEData[2U + i] = (uint8) *addr64;
                    protocol->ucFRAMEData[2U + i + 1U] = (uint8) (*addr64 >> 8);
                    protocol->ucFRAMEData[2U + i + 2U] = (uint8) (*addr64 >> 16);
                    protocol->ucFRAMEData[2U + i + 3U] = (uint8) (*addr64 >> 24);
                    protocol->ucFRAMEData[2U + i + 4U] = (uint8) (*addr64 >> 32);
                    protocol->ucFRAMEData[2U + i + 5U] = (uint8) (*addr64 >> 40);
                    protocol->ucFRAMEData[2U + i + 6U] = (uint8) (*addr64 >> 48);
                    protocol->ucFRAMEData[2U + i + 7U] = (uint8) (*addr64 >> 56);
                    addr64++;
            	}
            	break;
            }
            default:
                errorID = ERRORFormat;
                break;
        }
    }

    if (errorID == ERRORSuccess)
    {
        protocol->ucFRAMESize = size + 2U;
        protocol->ucFRAMEData[1] = errorID;
        protocol->pSnd_Enable(protocol);
    }
    else
    {
        sendError(protocol, errorID);
    }
}

/**
 * @brief Downloads block data to target.
 *
 * This services downloads block data by using the blocks' global address.
 * The block save function is executed if the block was found within the function table.
 *
 * @note This service is not protected by payload length protection.
 *       It's not required because response frame length <= request frame.
 */
void putBlockData(tProtocol* protocol)
{
    uint16 id, i;
    uint16* addr;

#if defined(__ADDRESS_WIDTH_8BIT__)
    const uint16 addrOffset = 2;
    addr = (uint16*) ((uint8) protocol->ucFRAMEData[1];
#elif defined(__ADDRESS_WIDTH_16BIT__)
    const uint16 addrOffset = 3;
    addr = (uint16*) ((uint16) protocol->ucFRAMEData[1] + ((uint16) protocol->ucFRAMEData[2] << 8));
#elif defined(__ADDRESS_WIDTH_32BIT__)
    const uint16 addrOffset = 5;
    addr = (uint16*) ((uint32) protocol->ucFRAMEData[1] + ((uint32) protocol->ucFRAMEData[2] << 8) +
    		((uint32) protocol->ucFRAMEData[3] << 16) + ((uint32) protocol->ucFRAMEData[4] << 24));
#else
#error ADDRESS WIDTH NOT DEFINED
#endif
    id = *addr;

    i = 0;
    while ((id != TableStruct->TFncTable[i].iBlockID) && (TableStruct->TFncTable[i].iBlockID != 0U))
    {
        i++;
    }
    if (TableStruct->TFncTable[i].iBlockID == 0U)
    {
        /* if correct block id was not found -> return wrong block id error */
        sendError(protocol, ERRORBlkID);
    } else {
        if (TableStruct->TFncTable[i].pFSave(addr, &protocol->ucFRAMEData[addrOffset], protocol->ucFRAMESize - addrOffset) != 0U)
        {
            sendError(protocol, ERRORFormat);
        }
        else
        {
            protocol->ucFRAMESize = 2;
            protocol->ucFRAMEData[1] = ERRORSuccess;
            protocol->pSnd_Enable(protocol);
        }
    }
}

/**
 * @brief Uploads block data from target.
 *
 * This services uploads block data by using the blocks' global address.
 * The block load function is executed if the block was found within the function table.
 *
 * @note This service is not protected by payload length protection.
 *       Implementation is currently not possible => see line with pFLoad instruction.
 */
void getBlockData(tProtocol* protocol)
{
    uint16 id, i;
    uint16* addr;

#if defined(__ADDRESS_WIDTH_8BIT__)
    	addr = (uint16*) ((uint8) protocol->ucFRAMEData[1];
#elif defined(__ADDRESS_WIDTH_16BIT__)
        addr = (uint16*) ((uint16) protocol->ucFRAMEData[1] + \
                ((uint16) protocol->ucFRAMEData[2] << 8));
#elif defined(__ADDRESS_WIDTH_32BIT__)
        addr = (uint16*) ((uint32) protocol->ucFRAMEData[1] + ((uint32) protocol->ucFRAMEData[2] << 8) + \
                ((uint32) protocol->ucFRAMEData[3] << 16) + ((uint32) protocol->ucFRAMEData[4] << 24));
#else
#error ADDRESS WIDTH NOT DEFINED
#endif
    id = *addr;

    i = 0;
    while ((id != TableStruct->TFncTable[i].iBlockID) && (TableStruct->TFncTable[i].iBlockID != 0U))
    {
        i++;
    }
    if (TableStruct->TFncTable[i].iBlockID == 0U)
    {
        /* if correct block id was not found -> return wrong block id error */
        sendError(protocol, ERRORBlkID);
    }
    else
    {
    	if (TableStruct->TFncTable[i].pFLoad(addr, &protocol->ucFRAMEData[2], &protocol->ucFRAMESize, protocol->ucMaxCommSize - 2U) != 0U)
    	{
    		sendError(protocol, ERRORFormat);
    	}
    	else
    	{
    		protocol->ucFRAMESize += 2U; /* add overhead size */
			protocol->ucFRAMEData[1] = ERRORSuccess;
			protocol->pSnd_Enable(protocol);
    	}
    }
}
