//
//  InputMemoryBitStream.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

InputMemoryBitStream::InputMemoryBitStream(char* buffer, uint16_t byteCount) :
_buffer(buffer),
_bitHead(0),
_bitCapacity(byteCount * 8),
_isBufferOwner(false)
{
    // Empty
}

InputMemoryBitStream::InputMemoryBitStream(const InputMemoryBitStream& other) :
_bitHead(other._bitHead),
_bitCapacity(other._bitCapacity),
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

uint16_t InputMemoryBitStream::getRemainingBitCount() const
{
    return _bitCapacity - _bitHead;
}

void InputMemoryBitStream::readBits(uint8_t& outData, uint16_t bitCount)
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

void InputMemoryBitStream::readBits(void* outData, uint16_t bitCount)
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

void InputMemoryBitStream::readBytes(void* outData, uint16_t byteCount)
{
    readBits(outData, byteCount << 3);
}

void InputMemoryBitStream::read(uint64_t& data)
{
    readBits(&data, 64);
    data = ntohll(data);
}

void InputMemoryBitStream::read(uint32_t& data)
{
    readBits(&data, 32);
    data = ntohl(data);
}

void InputMemoryBitStream::read(uint16_t& data)
{
    readBits(&data, 16);
    data = ntohs(data);
}

void InputMemoryBitStream::read(uint8_t& data)
{
    readBits(&data, 8);
}

void InputMemoryBitStream::read(int64_t& data)
{
    readBits(&data, 64);
    data = ntohll(data);
}

void InputMemoryBitStream::read(int32_t& data)
{
    readBits(&data, 32);
    data = ntohl(data);
}

void InputMemoryBitStream::read(int16_t& data)
{
    readBits(&data, 16);
    data = ntohs(data);
}

void InputMemoryBitStream::read(int8_t& data)
{
    readBits(&data, 8);
}

void InputMemoryBitStream::read(double& data)
{
    readBits(&data, 64);
    data = ntoh_double(data);
}

void InputMemoryBitStream::read(float& data)
{
    readBits(&data, 32);
    data = ntoh_float(data);
}

void InputMemoryBitStream::read(char& data)
{
    readBits(&data, 8);
}

void InputMemoryBitStream::read(bool& data)
{
    readBits(&data, 1);
}

void InputMemoryBitStream::resetToCapacity(uint16_t byteCapacity)
{
    _bitCapacity = byteCapacity << 3;
    _bitHead = 0;
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
