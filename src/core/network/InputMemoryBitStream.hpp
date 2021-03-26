//
//  InputMemoryBitStream.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <cstdlib>
#include <string>

class InputMemoryBitStream
{
public:
    InputMemoryBitStream(char* buffer, uint32_t byteCount);
    InputMemoryBitStream(const InputMemoryBitStream& other);
    ~InputMemoryBitStream();
    
    const char*	getBufferPtr() const;
    uint32_t getRemainingBitCount() const;
    void readBits(uint8_t& outData, uint32_t bitCount);
    void readBits(void* outData, uint32_t bitCount);
    void readBytes(void* outData, uint32_t byteCount);
    void read(uint64_t& data);
    void read(uint32_t& data);
    void read(uint16_t& data);
    void read(uint8_t& data);
    void read(int64_t& data);
    void read(int32_t& data);
    void read(int16_t& data);
    void read(int8_t& data);
    void read(double& data);
    void read(float& data);
    void read(char& data);
    void read(bool& data);
    void resetToCapacity(uint32_t byteCapacity);
    void readLarge(std::string& value);
    void readSmall(std::string& value);
    
private:
    char* _buffer;
    uint32_t _bitHead;
    uint32_t _bitCapacity;
    bool _isBufferOwner;
};
