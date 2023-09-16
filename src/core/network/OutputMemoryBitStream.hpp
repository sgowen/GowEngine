//
//  OutputMemoryBitStream.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <cstdlib>
#include <string>

class OutputMemoryBitStream
{
public:
    OutputMemoryBitStream(uint16_t initialBufferByteSize);
    ~OutputMemoryBitStream();
    
    const char* getBufferPtr() const;
    uint16_t getBitLength() const;
    uint16_t getByteLength() const;
    void writeBits(uint8_t data, uint16_t bitCount);
    void writeBits(const void* data, uint16_t bitCount);
    void writeBytes(const void* data, uint16_t byteCount);
    void write(uint64_t data);
    void write(uint32_t data);
    void write(uint16_t data);
    void write(uint8_t data);
    void write(int64_t data);
    void write(int32_t data);
    void write(int16_t data);
    void write(int8_t data);
    void write(double data);
    void write(float data);
    void write(char data);
    void write(bool data);
    void writeLarge(const std::string& value);
    void writeSmall(const std::string& value);
    
private:
    char* _buffer;
    uint16_t _bitHead;
    uint16_t _bitCapacity;
    
    void reallocBuffer(uint16_t newBitCapacity);
};
