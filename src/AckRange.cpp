//
//  AckRange.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AckRange.hpp"

#include "OutputMemoryBitStream.hpp"
#include "InputMemoryBitStream.hpp"

AckRange::AckRange() :
_start(0),
_count(0)
{
    // Empty
}

AckRange::AckRange(uint16_t start) :
_start(start),
_count(1)
{
    // Empty
}

void AckRange::write(OutputMemoryBitStream& outputStream) const
{
    outputStream.write(_start);
    bool hasCount = _count > 1;
    outputStream.write(hasCount);
    if (hasCount)
    {
        //most you can ack is 255...
        uint32_t countMinusOne = _count - 1;
        uint8_t countToAck = countMinusOne > 255 ? 255 : static_cast<uint8_t>(countMinusOne);
        outputStream.write(countToAck);
    }
}

void AckRange::read(InputMemoryBitStream& inputStream)
{
    inputStream.read(_start);
    bool hasCount;
    inputStream.read(hasCount);
    if (hasCount)
    {
        uint8_t countMinusOne;
        inputStream.read(countMinusOne);
        _count = countMinusOne + 1;
    }
    else
    {
        //default!
        _count = 1;
    }
}

bool AckRange::extendIfShould(uint16_t inSequenceNumber)
{
    if (inSequenceNumber == _start + _count)
    {
        ++_count;
        return true;
    }
    else
    {
        return false;
    }
}

uint16_t AckRange::getStart() const
{
    return _start;
}

uint32_t AckRange::getCount() const
{
    return _count;
}
