//
//  OutputMemoryBitStream.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

OutputMemoryBitStream::OutputMemoryBitStream(uint16_t initialBufferByteSize) :
_buffer(nullptr),
_bitHead(0)
{
    reallocBuffer(initialBufferByteSize * 8);
}

OutputMemoryBitStream::~OutputMemoryBitStream()
{
    std::free(_buffer);
}

const char* OutputMemoryBitStream::getBufferPtr() const
{
    return _buffer;
}

uint16_t OutputMemoryBitStream::getBitLength() const
{
    return _bitHead;
}

uint16_t OutputMemoryBitStream::getByteLength() const
{
    return (_bitHead + 7) >> 3;
}

void OutputMemoryBitStream::writeBits(uint8_t data, uint16_t bitCount)
{
    uint32_t nextBitHead = _bitHead + bitCount;

    if (nextBitHead > _bitCapacity)
    {
        reallocBuffer(GOW_MAX(_bitCapacity * 2, nextBitHead));
    }
    
    uint32_t byteOffset = _bitHead >> 3;
    uint32_t bitOffset = _bitHead & 0x7;

    uint8_t currentMask = ~(0xff << bitOffset);
    _buffer[byteOffset] = (_buffer[byteOffset] & currentMask) | (data << bitOffset);
    
    uint32_t bitsFreeThisByte = 8 - bitOffset;
    
    if (bitsFreeThisByte < bitCount)
    {
        _buffer[byteOffset + 1] = data >> bitsFreeThisByte;
    }

    _bitHead = nextBitHead;
}

void OutputMemoryBitStream::writeBits(const void* data, uint16_t bitCount)
{
    const char* srcByte = static_cast<const char*>(data);
    
    while (bitCount > 8)
    {
        writeBits(*srcByte, 8);
        ++srcByte;
        bitCount -= 8;
    }
    
    if (bitCount > 0)
    {
        writeBits(*srcByte, bitCount);
    }
}

void OutputMemoryBitStream::writeBytes(const void* data, uint16_t byteCount)
{
    writeBits(data, byteCount << 3);
}

void OutputMemoryBitStream::write(uint64_t data)
{
    uint64_t toWrite = htonll(data);
    writeBits(&toWrite, 64);
}

void OutputMemoryBitStream::write(uint32_t data)
{
    uint32_t toWrite = htonl(data);
    writeBits(&toWrite, 32);
}

void OutputMemoryBitStream::write(uint16_t data)
{
    uint16_t toWrite = htons(data);
    writeBits(&toWrite, 16);
}

void OutputMemoryBitStream::write(uint8_t data)
{
    writeBits(&data, 8);
}

void OutputMemoryBitStream::write(int64_t data)
{
    int64_t toWrite = htonll(data);
    writeBits(&toWrite, 64);
}

void OutputMemoryBitStream::write(int32_t data)
{
    int32_t toWrite = htonl(data);
    writeBits(&toWrite, 32);
}

void OutputMemoryBitStream::write(int16_t data)
{
    int16_t toWrite = htons(data);
    writeBits(&toWrite, 16);
}

void OutputMemoryBitStream::write(int8_t data)
{
    writeBits(&data, 8);
}

void OutputMemoryBitStream::write(double data)
{
    double toWrite = hton_double(data);
    writeBits(&toWrite, 64);
}

void OutputMemoryBitStream::write(float data)
{
    float toWrite = hton_float(data);
    writeBits(&toWrite, 32);
}

void OutputMemoryBitStream::write(char data)
{
    writeBits(&data, 8);
}

void OutputMemoryBitStream::write(bool data)
{
    writeBits(&data, 1);
}

void OutputMemoryBitStream::writeLarge(const std::string& value)
{
    uint16_t elementCount = static_cast<uint16_t>(value.size());
    assert(elementCount <= 1198);
    write(elementCount);
    for (const char& element : value)
    {
        write(element);
    }
}

void OutputMemoryBitStream::writeSmall(const std::string& value)
{
    uint8_t elementCount = static_cast<uint8_t>(value.size());
    write(elementCount);
    for (const char& element : value)
    {
        write(element);
    }
}

void OutputMemoryBitStream::reallocBuffer(uint16_t newBitLength)
{
    if (_buffer == nullptr)
    {
        _buffer = static_cast<char*>(std::malloc(newBitLength >> 3));
        memset(_buffer, 0, newBitLength >> 3);
    }
    else
    {
        char* tempBuffer = static_cast<char*>(std::malloc(newBitLength >> 3));
        memset(tempBuffer, 0, newBitLength >> 3);
        memcpy(tempBuffer, _buffer, _bitCapacity >> 3);
        std::free(_buffer);
        _buffer = tempBuffer;
    }
    
    _bitCapacity = newBitLength;
}
