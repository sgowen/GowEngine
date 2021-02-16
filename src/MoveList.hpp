//
//  MoveList.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Move.hpp"

#include <deque>

class InputState;

typedef void (*InputStateReleaseFunc)(InputState* inputState);

class MoveList
{
public:
    MoveList();
    
    const Move& addMove(InputState* inputState, uint32_t timestamp, uint32_t index);
    bool addMoveIfNew(const Move& move);
    void markMoveAsProcessed(Move* move);
    void removeProcessedMoves(uint32_t lastMoveProcessedOnServerTimestamp, InputStateReleaseFunc inputStateReleaseFunc);
    uint32_t getLastMoveTimestamp() const;
    uint32_t getLastProcessedMoveTimestamp() const;
    void clear();
    bool hasMoves() const;
    int getMoveCount() const;
    int getNumMovesAfterTimestamp(uint32_t lastMoveReceivedOnServerTimestamp) const;
    Move* getMoveAtIndex(int index);
    
    std::deque<Move>::const_iterator begin() const;
    std::deque<Move>::const_iterator end() const;
    
private:
    uint32_t _lastMoveTimestamp;
    uint32_t _lastProcessedMoveTimestamp;
    std::deque<Move> _moves;
};
