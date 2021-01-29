//
//  InputMemoryBitStream.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Network.hpp"

#include <stdint.h>
#include <cstdlib>
#include <string>

class InputMemoryBitStream
{
public:
    InputMemoryBitStream(char* buffer, uint32_t bitCount);
    InputMemoryBitStream(const InputMemoryBitStream& other);
    ~InputMemoryBitStream();
    
    const char*	getBufferPtr() const;
    uint32_t getRemainingBitCount() const;
    void readBits(uint8_t& outData, uint32_t bitCount);
    void readBits(void* outData, uint32_t bitCount);
    void readBytes(void* outData, uint32_t byteCount);
    void read(bool& outData);
    void resetToCapacity(uint32_t byteCapacity);
    void readLarge(std::string& value);
    void readSmall(std::string& value);
    
    template <typename T, uint32_t BIT_COUNT = sizeof(T) * 8>
    void read(T& outData)
    {
        static_assert(std::is_arithmetic< T >::value, "Generic Read only supports primitive data types");
        
        static_assert(BIT_COUNT == 64 || BIT_COUNT == 32 || BIT_COUNT == 16 || BIT_COUNT <= 8, "BIT_COUNT must be 64, 32, 16, or less than or equal to 8");
        
        readBits(&outData, BIT_COUNT);
        
        if (BIT_COUNT == 64)
        {
            outData = ntohll(outData);
        }
        else if (BIT_COUNT == 32)
        {
            if (std::is_floating_point<T>::value)
            {
                outData = ntoh_float(outData);
            }
            else
            {
                outData = ntohl(outData);
            }
        }
        else if (BIT_COUNT == 16)
        {
            outData = ntohs(outData);
        }
    }
    
private:
    char* _buffer;
    uint32_t _bitHead;
    uint32_t _bitCapacity;
    bool _isBufferOwner;
};
