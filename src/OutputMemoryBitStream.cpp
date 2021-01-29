//
//  OutputMemoryBitStream.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "OutputMemoryBitStream.hpp"

#include "MathUtil.hpp"
#include "StringUtil.hpp"
#include "Constants.hpp"

#include <cstring>
#include <assert.h>

OutputMemoryBitStream::OutputMemoryBitStream() :
_bitHead(0),
_buffer(NULL)
{
    reallocBuffer(NW_MAX_PACKET_SIZE * 8);
}

OutputMemoryBitStream::~OutputMemoryBitStream()
{
    std::free(_buffer);
}

const char* OutputMemoryBitStream::getBufferPtr() const
{
    return _buffer;
}

uint32_t OutputMemoryBitStream::getBitLength() const
{
    return _bitHead;
}

uint32_t OutputMemoryBitStream::getByteLength() const
{
    return (_bitHead + 7) >> 3;
}

void OutputMemoryBitStream::writeBits(uint8_t data, uint32_t bitCount)
{
    uint32_t nextBitHead = _bitHead + static_cast<uint32_t>(bitCount);

    if (nextBitHead > _bitCapacity)
    {
        reallocBuffer(std::max(_bitCapacity * 2, nextBitHead));
    }

    //calculate the byteOffset into our buffer
    //by dividing the head by 8
    //and the bitOffset by taking the last 3 bits
    uint32_t byteOffset = _bitHead >> 3;
    uint32_t bitOffset = _bitHead & 0x7;

    uint8_t currentMask = ~(0xff << bitOffset);
    _buffer[byteOffset] = (_buffer[byteOffset] & currentMask) | (data << bitOffset);

    //calculate how many bits were not yet used in
    //our target byte in the buffer
    uint32_t bitsFreeThisByte = 8 - bitOffset;

    //if we needed more than that, carry to the next byte
    if (bitsFreeThisByte < bitCount)
    {
        //we need another byte
        _buffer[byteOffset + 1] = data >> bitsFreeThisByte;
    }

    _bitHead = nextBitHead;
}

void OutputMemoryBitStream::writeBits(const void* data, uint32_t bitCount)
{
    const char* srcByte = static_cast<const char*>(data);
    //write all the bytes
    while (bitCount > 8)
    {
        writeBits(*srcByte, 8);
        ++srcByte;
        bitCount -= 8;
    }
    //write anything left
    if (bitCount > 0)
    {
        writeBits(*srcByte, bitCount);
    }
}

void OutputMemoryBitStream::writeBytes(const void* data, uint32_t byteCount)
{
    writeBits(data, byteCount << 3);
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
    for (const auto& element : value)
    {
        write(element);
    }
}

void OutputMemoryBitStream::writeSignedBinaryValue(float value)
{
    bool isNonZero = (value != 0.f);
    write(isNonZero);
    if (isNonZero)
    {
        write(value > 0.f);
    }
}

void OutputMemoryBitStream::reallocBuffer(uint32_t newBitLength)
{
    if (_buffer == NULL)
    {
        //just need to memset on first allocation
        _buffer = static_cast<char*>(std::malloc(newBitLength >> 3));
        memset(_buffer, 0, newBitLength >> 3);
    }
    else
    {
        //need to memset, then copy the buffer
        char* tempBuffer = static_cast<char*>(std::malloc(newBitLength >> 3));
        memset(tempBuffer, 0, newBitLength >> 3);
        memcpy(tempBuffer, _buffer, _bitCapacity >> 3);
        std::free(_buffer);
        _buffer = tempBuffer;
    }

    //handle realloc failure
    //...
    _bitCapacity = newBitLength;
}
