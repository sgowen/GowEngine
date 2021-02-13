//
//  Move.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Move.hpp"

#include "InputState.hpp"
#include "OutputMemoryBitStream.hpp"
#include "InputMemoryBitStream.hpp"

Move::Move(InputState* inputState, float timestamp) :
_inputState(inputState),
_timestamp(timestamp)
{
    // Empty
}

Move::Move(InputState* inputState) :
_inputState(inputState),
_timestamp(0)
{
    // Empty
}

void Move::write(OutputMemoryBitStream& ombs) const
{
    ombs.write(_timestamp);
    
    _inputState->write(ombs);
}

void Move::read(InputMemoryBitStream& imbs)
{
    imbs.read(_timestamp);
    
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

float Move::getTimestamp() const
{
    return _timestamp;
}

void Move::setTimestamp(float timeStamp)
{
    _timestamp = timeStamp;
}

void Move::copyInputState(InputState* inputState)
{
    inputState->copyTo(_inputState);
}
