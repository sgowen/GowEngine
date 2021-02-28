//
//  InputState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
        friend class InputState;
        
    public:
        uint8_t _playerID;
        uint8_t _inputState;
        
        PlayerInputState();
        
        void write(OutputMemoryBitStream& ombs) const;
        void read(InputMemoryBitStream& imbs);
    };
    
    InputState();
    
    void setMaxNumPlayers(uint8_t maxNumPlayers);
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
    std::vector<PlayerInputState> _playerInputStates;
    uint8_t _maxNumPlayers;
};
