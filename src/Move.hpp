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
    Move(InputState* inputState, uint32_t timestamp);
    Move(InputState* inputState);
    ~Move() {}
    
    void write(OutputMemoryBitStream& ombs) const;
    void read(InputMemoryBitStream& imbs);
    bool isEqual(const Move* move) const;
    InputState* inputState() const;
    uint32_t getTimestamp() const;
    void setTimestamp(uint32_t timeStamp);
    void copyInputState(InputState* inputState);
    
private:
    InputState* _inputState;
    uint32_t _timestamp;
};
