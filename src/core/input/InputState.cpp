//
//  InputState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define INACTIVE_PLAYER_ID 0
#define NO_INPUT 0

InputState::InputState()
{
    _playerInputStates.resize(MAX_NUM_PLAYERS);
}

void InputState::write(OutputMemoryBitStream& ombs) const
{
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        bool isInputAssigned = _playerInputStates[i]._playerID != INACTIVE_PLAYER_ID;
        ombs.write(isInputAssigned);
        if (isInputAssigned)
        {
            _playerInputStates[i].write(ombs);
        }
    }
}

void InputState::read(InputMemoryBitStream& imbs)
{
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        bool isInputAssigned;
        imbs.read(isInputAssigned);
        if (isInputAssigned)
        {
            _playerInputStates[i].read(imbs);
        }
    }
}

void InputState::reset()
{
    for (PlayerInputState& pis : _playerInputStates)
    {
        pis._playerID = INACTIVE_PLAYER_ID;
        pis._inputState = NO_INPUT;
    }
}

bool InputState::isEqual(InputState* inputState) const
{
    InputState* is = static_cast<InputState*>(inputState);
    
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        if (is->_playerInputStates[i]._playerID != _playerInputStates[i]._playerID)
        {
            return false;
        }
        
        if (is->_playerInputStates[i]._inputState != _playerInputStates[i]._inputState)
        {
            return false;
        }
    }
    
    return true;
}

void InputState::copyTo(InputState* inputState) const
{
    InputState* is = static_cast<InputState*>(inputState);
    
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        is->_playerInputStates[i]._playerID = _playerInputStates[i]._playerID;
        is->_playerInputStates[i]._inputState = _playerInputStates[i]._inputState;
    }
}

void InputState::activateNextPlayer(uint8_t playerID)
{
    assert(playerID != INACTIVE_PLAYER_ID);
    
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        if (_playerInputStates[i]._playerID == INACTIVE_PLAYER_ID)
        {
            _playerInputStates[i]._playerID = playerID;
            return;
        }
    }
}

InputState::PlayerInputState* InputState::playerInputStateForID(uint8_t playerID)
{
    for (uint8_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        if (_playerInputStates[i]._playerID == playerID)
        {
            return &playerInputState(i);
        }
    }
    
    return nullptr;
}

bool InputState::isRequestingToAddLocalPlayer() const
{
    for (int i = 1; i < MAX_NUM_PLAYERS; ++i)
    {
        if (_playerInputStates[i]._playerID == INACTIVE_PLAYER_ID &&
            _playerInputStates[i]._inputState != NO_INPUT)
        {
            return true;
        }
    }
    
    return false;
}

InputState::PlayerInputState& InputState::playerInputState(uint8_t index)
{
    return _playerInputStates[index];
}

void InputState::PlayerInputState::write(OutputMemoryBitStream& ombs) const
{
    ombs.writeBits(_playerID, 3);
    ombs.write(_inputState);
}

void InputState::PlayerInputState::read(InputMemoryBitStream& imbs)
{
    imbs.readBits(_playerID, 3);
    imbs.read(_inputState);
}

InputState::PlayerInputState::PlayerInputState() :
_playerID(0),
_inputState(0)
{
    // Empty
}
