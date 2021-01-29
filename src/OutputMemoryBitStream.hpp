//
//  OutputMemoryBitStream.hpp
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

class OutputMemoryBitStream
{
public:
    OutputMemoryBitStream();
    ~OutputMemoryBitStream();
    
    const char* getBufferPtr() const;
    uint32_t getBitLength() const;
    uint32_t getByteLength() const;
    void writeBits(uint8_t data, uint32_t bitCount);
    void writeBits(const void* data, uint32_t bitCount);
    void writeBytes(const void* data, uint32_t byteCount);
    void write(bool data);
    void writeLarge(const std::string& value);
    void writeSmall(const std::string& value);
    void writeSignedBinaryValue(float value);
    
    template <typename T, uint32_t BIT_COUNT = sizeof(T) * 8>
    void write(T data)
    {
        static_assert(std::is_arithmetic< T >::value, "Generic Write only supports primitive data types");
        
        static_assert(BIT_COUNT == 64 || BIT_COUNT == 32 || BIT_COUNT == 16 || BIT_COUNT <= 8, "BIT_COUNT must be 64, 32, 16, or less than or equal to 8");
        
        T toWrite = data;
        
        if (BIT_COUNT == 64)
        {
            toWrite = htonll(data);
        }
        else if (BIT_COUNT == 32)
        {
            if (std::is_floating_point<T>::value)
            {
                toWrite = hton_float(data);
            }
            else
            {
                toWrite = htonl(data);
            }
        }
        else if (BIT_COUNT == 16)
        {
            toWrite = htons(data);
        }
        
        writeBits(&toWrite, BIT_COUNT);
    }
    
private:
    void reallocBuffer(uint32_t newBitCapacity);
    
    char* _buffer;
    uint32_t _bitHead;
    uint32_t _bitCapacity;
};
