//
//  InputState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>

class OutputMemoryBitStream;
class InputMemoryBitStream;

class InputState
{
public:
    class PlayerInputState
    {        
    public:
        uint8_t _playerID;
        uint16_t _inputState;
        
        PlayerInputState();
        
        void write(OutputMemoryBitStream& ombs) const;
        void read(InputMemoryBitStream& imbs);
    };
    
    void write(OutputMemoryBitStream& ombs) const;
    void read(InputMemoryBitStream& imbs);
    void reset();
    bool isEqual(InputState* inputState) const;
    void copyTo(InputState* inputState) const;
    void activateNextPlayer(uint8_t playerID);
    PlayerInputState* playerInputStateForID(uint8_t playerID);
    bool isRequestingToAddLocalPlayer() const;
    PlayerInputState& playerInputState(uint8_t index);
    
private:
    PlayerInputState _playerInputStates[MAX_NUM_PLAYERS];
};
