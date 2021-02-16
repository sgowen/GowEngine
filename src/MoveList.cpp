//
//  MoveList.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "MoveList.hpp"

#include "InputState.hpp"
#include "TimeTracker.hpp"
#include "StringUtil.hpp"

MoveList::MoveList() :
_lastMoveTimestamp(0),
_lastProcessedMoveTimestamp(0)
{
    // Empty
}

const Move& MoveList::addMove(InputState* inputState, uint32_t timestamp, uint32_t index)
{
    _moves.emplace_back(inputState, timestamp, index);
    
    _lastMoveTimestamp = timestamp;
    
    return _moves.back();
}

bool MoveList::addMoveIfNew(const Move& move)
{
    //we might have already received this move in another packet (since we're sending the same move in multiple packets)
    //so make sure it's new...
    
    //adjust the deltatime and then place!
    uint32_t timeStamp = move.getTimestamp();
    uint32_t index = move.getIndex();
    
    if (timeStamp > _lastMoveTimestamp)
    {
        _lastMoveTimestamp = timeStamp;
        
        _moves.emplace_back(move.inputState(), timeStamp, index);
        
        return true;
    }
    
    return false;
}

void MoveList::markMoveAsProcessed(Move* move)
{
    _lastProcessedMoveTimestamp = move->getTimestamp();
}

void MoveList::removeProcessedMoves(uint32_t lastMoveProcessedOnServerTimestamp, InputStateReleaseFunc inputStateReleaseFunc)
{
    while (!_moves.empty() && _moves.front().getTimestamp() <= lastMoveProcessedOnServerTimestamp)
    {
        inputStateReleaseFunc(_moves.front().inputState());

        _moves.pop_front();
    }
}

uint32_t MoveList::getLastMoveTimestamp() const
{
    return _lastMoveTimestamp;
}

uint32_t MoveList::getLastProcessedMoveTimestamp() const
{
    return _lastProcessedMoveTimestamp;
}

void MoveList::clear()
{
    _moves.clear();
}

bool MoveList::hasMoves() const
{
    return !_moves.empty();
}

int MoveList::getMoveCount() const
{
    return static_cast<int>(_moves.size());
}

int MoveList::getNumMovesAfterTimestamp(uint32_t lastMoveReceivedOnServerTimestamp) const
{
    int ret = 0;
    
    for (Move move : _moves)
    {
        if (move.getTimestamp() > lastMoveReceivedOnServerTimestamp)
        {
            ++ret;
        }
    }
    
    return ret;
}

Move* MoveList::getMoveAtIndex(int index)
{
    if (index < getMoveCount())
    {
        return &_moves.at(index);
    }
    
    return NULL;
}

std::deque<Move>::const_iterator MoveList::begin() const
{
    return _moves.begin();
}

std::deque<Move>::const_iterator MoveList::end() const
{
    return _moves.end();
}
