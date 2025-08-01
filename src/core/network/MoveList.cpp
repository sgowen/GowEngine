//
//  MoveList.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

MoveList::MoveList() :
_lastMoveTimestamp(0),
_lastMoveIndex(0),
_lastProcessedMoveTimestamp(0)
{
    // Empty
}

void MoveList::addMove(InputState* inputState, uint32_t timestamp, uint32_t index)
{
    _moves.emplace_back(inputState, timestamp, index);
    
    _lastMoveTimestamp = timestamp;
    _lastMoveIndex = index;
}

bool MoveList::addMoveIfNew(const Move& move)
{
    uint32_t timeStamp = move.getTimestamp();
    uint32_t index = move.getIndex();
    
    if (timeStamp > _lastMoveTimestamp)
    {
        _lastMoveTimestamp = timeStamp;
        _lastMoveIndex = index;
        
        _moves.emplace_back(move.inputState(), timeStamp, index);
        
        return true;
    }
    
    return false;
}

void MoveList::markMoveAsProcessed(Move* move)
{
    _lastProcessedMoveTimestamp = move->getTimestamp();
}

bool MoveList::isMoveProcessed(Move* move)
{
    return _lastProcessedMoveTimestamp >= move->getTimestamp();
}

void MoveList::removeProcessedMoves(Pool<InputState>& inputStatePool)
{
    removeProcessedMovesAtOrBeforeTimestamp(_lastProcessedMoveTimestamp, inputStatePool);
}

void MoveList::removeProcessedMovesAtOrBeforeTimestamp(uint32_t lastMoveProcessedOnServerTimestamp, Pool<InputState>& inputStatePool)
{
    while (!_moves.empty() && _moves.front().getTimestamp() <= lastMoveProcessedOnServerTimestamp)
    {
        inputStatePool.free(_moves.front().inputState());

        _moves.pop_front();
    }
}

void MoveList::removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed, Pool<InputState>& inputStatePool)
{
    while (!_moves.empty() && _moves.front().getIndex() < numMovesProcessed)
    {
        inputStatePool.free(_moves.front().inputState());

        _moves.pop_front();
    }
}

void MoveList::removeAllMoves(Pool<InputState>& inputStatePool)
{
    while (!_moves.empty())
    {
        inputStatePool.free(_moves.front().inputState());

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
    
    return nullptr;
}

Move* MoveList::getMoveWithMoveIndex(uint32_t moveIndex)
{
    for (Move& move : _moves)
    {
        if (move.getIndex() == moveIndex)
        {
            return &move;
        }
    }
    
    return nullptr;
}

std::deque<Move>::const_iterator MoveList::begin() const
{
    return _moves.begin();
}

std::deque<Move>::const_iterator MoveList::end() const
{
    return _moves.end();
}
