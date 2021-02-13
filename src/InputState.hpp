//
//  InputState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class OutputMemoryBitStream;
class InputMemoryBitStream;

class InputState
{
public:
    InputState() {}
    virtual ~InputState() {}
    
    virtual void write(OutputMemoryBitStream& ombs) const = 0;
    virtual void read(InputMemoryBitStream& imbs) = 0;
    virtual void reset() = 0;
    virtual bool isEqual(InputState* inputState) const = 0;
    virtual void copyTo(InputState* inputState) const = 0;
};
