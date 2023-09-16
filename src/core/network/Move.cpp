//
//  Move.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Move::Move(InputState* inputState, uint32_t timestamp, uint32_t index) :
_inputState(inputState),
_timestamp(timestamp),
_index(index)
{
    // Empty
}

Move::Move(InputState* inputState) :
_inputState(inputState),
_timestamp(0),
_index(0)
{
    // Empty
}

void Move::write(OutputMemoryBitStream& ombs) const
{
    ombs.write(_timestamp);
    ombs.write(_index);
    
    _inputState->write(ombs);
}

void Move::read(InputMemoryBitStream& imbs)
{
    imbs.read(_timestamp);
    imbs.read(_index);
    
    _inputState->read(imbs);
}

InputState* Move::inputState() const
{
    return _inputState;
}

uint32_t Move::getTimestamp() const
{
    return _timestamp;
}

uint32_t Move::getIndex() const
{
    return _index;
}
