//
//  InputState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "InputState.hpp"

#include "OutputMemoryBitStream.hpp"
#include "InputMemoryBitStream.hpp"
#include "Network.hpp"

#include <assert.h>

#define DEFAULT_MAX_NUM_PLAYERS 4

InputState::InputState() :
_maxNumPlayers(DEFAULT_MAX_NUM_PLAYERS)
{
    _playerInputStates.resize(_maxNumPlayers);
}

void InputState::setMaxNumPlayers(uint8_t maxNumPlayers)
{
    _maxNumPlayers = maxNumPlayers;
    _playerInputStates.resize(_maxNumPlayers);
}

void InputState::write(OutputMemoryBitStream& ombs) const
{
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
    {
        bool isInputAssigned = _playerInputStates[i]._playerID != NW_INPUT_UNASSIGNED;
        ombs.write(isInputAssigned);
        if (isInputAssigned)
        {
            _playerInputStates[i].write(ombs);
        }
    }
}

void InputState::read(InputMemoryBitStream& imbs)
{
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
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
    _maxNumPlayers = DEFAULT_MAX_NUM_PLAYERS;
    
    for (PlayerInputState& pis : _playerInputStates)
    {
        pis._playerID = NW_INPUT_UNASSIGNED;
        pis._inputState = 0;
    }
}

bool InputState::isEqual(InputState* inputState) const
{
    InputState* is = static_cast<InputState*>(inputState);
    
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
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
    
    is->_maxNumPlayers = _maxNumPlayers;
    
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
    {
        is->_playerInputStates[i]._playerID = _playerInputStates[i]._playerID;
        is->_playerInputStates[i]._inputState = _playerInputStates[i]._inputState;
    }
}

void InputState::activateNextPlayer(uint8_t playerID)
{
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
    {
        if (_playerInputStates[i]._playerID == NW_INPUT_UNASSIGNED)
        {
            _playerInputStates[i]._playerID = playerID;
            return;
        }
    }
}

InputState::PlayerInputState* InputState::playerInputStateForID(uint8_t playerID)
{
    for (uint8_t i = 0; i < _maxNumPlayers; ++i)
    {
        if (_playerInputStates[i]._playerID == playerID)
        {
            return &playerInputState(i);
        }
    }
    
    return NULL;
}

bool InputState::isRequestingToAddLocalPlayer() const
{
    for (int i = 1; i < _maxNumPlayers; ++i)
    {
        if (_playerInputStates[i]._playerID == NW_INPUT_UNASSIGNED &&
            _playerInputStates[i]._inputState > 0)
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
    ombs.write<uint8_t, 3>(_playerID);
    ombs.write(_inputState);
}

void InputState::PlayerInputState::read(InputMemoryBitStream& imbs)
{
    imbs.read<uint8_t, 3>(_playerID);
    imbs.read(_inputState);
}

InputState::PlayerInputState::PlayerInputState() :
_playerID(NW_INPUT_UNASSIGNED),
_inputState(0)
{
    // Empty
}
