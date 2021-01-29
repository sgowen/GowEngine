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

void Move::write(OutputMemoryBitStream& outputStream) const
{
    outputStream.write(_timestamp);
    
    _inputState->write(outputStream);
}

void Move::read(InputMemoryBitStream& inputStream)
{
    inputStream.read(_timestamp);
    
    _inputState->read(inputStream);
}

bool Move::isEqual(const Move* move) const
{
    return _inputState->isEqual(move->getInputState());
}

InputState* Move::getInputState() const
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
