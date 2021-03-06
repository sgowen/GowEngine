//
//  Move.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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

bool Move::isEqual(const Move* move) const
{
    return _inputState->isEqual(move->inputState());
}

InputState* Move::inputState() const
{
    return _inputState;
}

uint32_t Move::getTimestamp() const
{
    return _timestamp;
}

void Move::setTimestamp(uint32_t timeStamp)
{
    _timestamp = timeStamp;
}

uint32_t Move::getIndex() const
{
    return _index;
}

void Move::setIndex(uint32_t index)
{
    _index = index;
}

void Move::copyInputState(InputState* inputState)
{
    inputState->copyTo(_inputState);
}
