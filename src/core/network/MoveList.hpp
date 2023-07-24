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

class MoveList
{
public:
    MoveList();
    
    void addMove(InputState* inputState, uint32_t timestamp, uint32_t index);
    bool addMoveIfNew(const Move& move);
    void markMoveAsProcessed(Move* move);
    bool isMoveProcessed(Move* move);
    void removeProcessedMoves(Pool<InputState>& inputStatePool);
    void removeProcessedMovesAtOrBeforeTimestamp(uint32_t lastMoveProcessedOnServerTimestamp, Pool<InputState>& inputStatePool);
    void removeProcessedMovesWithIndexLessThan(uint32_t numMovesProcessed, Pool<InputState>& inputStatePool);
    void removeAllMoves(Pool<InputState>& inputStatePool);
    uint32_t getLastMoveTimestamp() const;
    uint32_t getLastProcessedMoveTimestamp() const;
    void clear();
    bool hasMoves() const;
    int getMoveCount() const;
    int getNumMovesAfterTimestamp(uint32_t lastMoveReceivedOnServerTimestamp) const;
    Move* getMoveAtIndex(int index);
    Move* getMoveWithMoveIndex(uint32_t index);
    
    std::deque<Move>::const_iterator begin() const;
    std::deque<Move>::const_iterator end() const;
    
private:
    uint32_t _lastMoveTimestamp;
    uint32_t _lastMoveIndex;
    uint32_t _lastProcessedMoveTimestamp;
    std::deque<Move> _moves;
};
