//
//  AckRange.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

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

void AckRange::write(OutputMemoryBitStream& ombs) const
{
    ombs.write(_start);
    bool hasCount = _count > 1;
    ombs.write(hasCount);
    if (hasCount)
    {
        uint32_t countMinusOne = _count - 1;
        uint8_t countToAck = countMinusOne > 255 ? 255 : static_cast<uint8_t>(countMinusOne);
        ombs.write(countToAck);
    }
}

void AckRange::read(InputMemoryBitStream& imbs)
{
    imbs.read(_start);
    bool hasCount;
    imbs.read(hasCount);
    if (hasCount)
    {
        uint8_t countMinusOne;
        imbs.read(countMinusOne);
        _count = countMinusOne + 1;
    }
    else
    {
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
