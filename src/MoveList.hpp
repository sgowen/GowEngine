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
    
    const Move& addMove(InputState* inputState, float timestamp);
    bool addMoveIfNew(const Move& move);
    void markMoveAsProcessed(Move* move);
    void removeProcessedMoves(float lastMoveProcessedOnServerTimestamp, InputStateReleaseFunc inputStateReleaseFunc);
    float getLastMoveTimestamp() const;
    float getLastProcessedMoveTimestamp() const;
    const Move& getLatestMove() const;
    void clear();
    bool hasMoves() const;
    int getMoveCount() const;
    int getNumMovesAfterTimestamp(float lastMoveReceivedOnServerTimestamp) const;
    Move* getMoveAtIndex(int index);
    
    std::deque<Move>::const_iterator begin() const;
    std::deque<Move>::const_iterator end() const;
    
private:
    float _lastMoveTimestamp;
    float _lastProcessedMoveTimestamp;
    std::deque<Move> _moves;
};
