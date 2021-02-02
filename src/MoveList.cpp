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
_lastMoveTimestamp(-1.0f),
_lastProcessedMoveTimestamp(-1.0f)
{
    // Empty
}

const Move& MoveList::addMove(InputState* inputState, float timestamp)
{
    _moves.push_back(Move(inputState, timestamp));
    
    _lastMoveTimestamp = timestamp;
    
    return _moves.back();
}

bool MoveList::addMoveIfNew(const Move& move)
{
    //we might have already received this move in another packet (since we're sending the same move in multiple packets)
    //so make sure it's new...
    
    //adjust the deltatime and then place!
    float timeStamp = move.getTimestamp();
    
    if (timeStamp > _lastMoveTimestamp)
    {
        _lastMoveTimestamp = timeStamp;
        
        _moves.push_back(Move(move.getInputState(), timeStamp));
        
        return true;
    }
    
    return false;
}

void MoveList::markMoveAsProcessed(Move* move)
{
    _lastProcessedMoveTimestamp = move->getTimestamp();
}

void MoveList::removeProcessedMoves(float lastMoveProcessedOnServerTimestamp, InputStateReleaseFunc inputStateReleaseFunc)
{
    while (!_moves.empty() && _moves.front().getTimestamp() <= lastMoveProcessedOnServerTimestamp)
    {
        inputStateReleaseFunc(_moves.front().getInputState());

        _moves.pop_front();
    }
}

float MoveList::getLastMoveTimestamp() const
{
    return _lastMoveTimestamp;
}

float MoveList::getLastProcessedMoveTimestamp() const
{
    return _lastProcessedMoveTimestamp;
}

const Move& MoveList::getLatestMove() const
{
    return _moves.back();
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

int MoveList::getNumMovesAfterTimestamp(float lastMoveReceivedOnServerTimestamp) const
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
