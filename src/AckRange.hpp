//
//  AckRange.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class OutputMemoryBitStream;
class InputMemoryBitStream;

class AckRange
{
public:
    AckRange();
    AckRange(uint16_t start);
    
    void write(OutputMemoryBitStream& outputStream) const;
    void read(InputMemoryBitStream& inputStream);
    bool extendIfShould(uint16_t inSequenceNumber);
    uint16_t getStart() const;
    uint32_t getCount() const;
    
private:
    uint16_t _start;
    uint32_t _count;
};
