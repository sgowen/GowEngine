//
//  Move.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class InputState;
class OutputMemoryBitStream;
class InputMemoryBitStream;

class Move
{
public:
    Move(InputState* inputState, uint32_t timestamp, uint32_t index);
    Move(InputState* inputState);
    
    void write(OutputMemoryBitStream& ombs) const;
    void read(InputMemoryBitStream& imbs);
    InputState* inputState() const;
    uint32_t getTimestamp() const;
    void setTimestamp(uint32_t timeStamp);
    uint32_t getIndex() const;
    void setIndex(uint32_t index);
    
private:
    InputState* _inputState;
    uint32_t _timestamp;
    uint32_t _index;
};
