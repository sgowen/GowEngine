//
//  InputMemoryBitStream.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "InputMemoryBitStream.hpp"

#include <cstring>

InputMemoryBitStream::InputMemoryBitStream(char* buffer, uint32_t byteCount) :
_buffer(buffer),
_bitCapacity(byteCount * 8),
_bitHead(0),
_isBufferOwner(false)
{
    // Empty
}

InputMemoryBitStream::InputMemoryBitStream(const InputMemoryBitStream& other) :
_bitCapacity(other._bitCapacity),
_bitHead(other._bitHead),
_isBufferOwner(true)
{
    int byteCount = (_bitCapacity + 7) / 8;
    _buffer = static_cast<char*>(malloc(byteCount));
    memcpy(_buffer, other._buffer, byteCount);
}

InputMemoryBitStream::~InputMemoryBitStream()
{
    if (_isBufferOwner)
    {
        free(_buffer);
    }
}

const char*	InputMemoryBitStream::getBufferPtr() const
{
    return _buffer;
}

uint32_t InputMemoryBitStream::getRemainingBitCount() const
{
    return _bitCapacity - _bitHead;
}

void InputMemoryBitStream::readBits(uint8_t& outData, uint32_t bitCount)
{
    uint32_t byteOffset = _bitHead >> 3;
    uint32_t bitOffset = _bitHead & 0x7;
    
    outData = static_cast<uint8_t>(_buffer[byteOffset]) >> bitOffset;
    
    uint32_t bitsFreeThisByte = 8 - bitOffset;
    if (bitsFreeThisByte < bitCount)
    {
        outData |= static_cast<uint8_t>(_buffer[byteOffset + 1]) << bitsFreeThisByte;
    }
    
    outData &= (~(0x00ff << bitCount));
    
    _bitHead += bitCount;
}

void InputMemoryBitStream::readBits(void* outData, uint32_t bitCount)
{
    uint8_t* destByte = reinterpret_cast< uint8_t* >(outData);
    
    while (bitCount > 8)
    {
        readBits(*destByte, 8);
        ++destByte;
        bitCount -= 8;
    }
    
    if (bitCount > 0)
    {
        readBits(*destByte, bitCount);
    }
}

void InputMemoryBitStream::readBytes(void* outData, uint32_t byteCount)
{
    readBits(outData, byteCount << 3);
}

void InputMemoryBitStream::read(bool& outData)
{
    readBits(&outData, 1);
}

void InputMemoryBitStream::resetToCapacity(uint32_t byteCapacity)
{
    _bitCapacity = byteCapacity << 3; _bitHead = 0;
}

void InputMemoryBitStream::readLarge(std::string& value)
{
    uint16_t elementCount;
    read(elementCount);
    
    value.resize(elementCount);
    for (auto& element : value)
    {
        read(element);
    }
}

void InputMemoryBitStream::readSmall(std::string& value)
{
    uint8_t elementCount;
    read(elementCount);
    
    value.resize(elementCount);
    for (auto& element : value)
    {
        read(element);
    }
}
