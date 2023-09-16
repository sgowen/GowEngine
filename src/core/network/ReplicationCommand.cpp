//
//  ReplicationCommand.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ReplicationCommand::ReplicationCommand() :
_action(REPA_CREATE),
_dirtyState(0)
{
    // Empty
}

ReplicationCommand::ReplicationCommand(uint8_t initialDirtyState) :
_action(REPA_CREATE),
_dirtyState(initialDirtyState)
{
    // Empty
}

void ReplicationCommand::handleCreateAckd()
{
    if (_action == REPA_CREATE)
    {
        _action = REPA_UPDATE;
    }
}

void ReplicationCommand::addDirtyState(uint8_t dirtyState)
{
    _dirtyState |= dirtyState;
}

void ReplicationCommand::setDestroy()
{
    _action = REPA_DESTROY;
}

bool ReplicationCommand::hasDirtyState() const
{
    return (_action == REPA_DESTROY) || (_dirtyState != 0);
}

ReplicationAction ReplicationCommand::getAction() const
{
    return _action;
}

uint8_t ReplicationCommand::getDirtyState() const
{
    return _dirtyState;
}

void ReplicationCommand::clearDirtyState()
{
    _dirtyState = 0;
    
    if (_action == REPA_DESTROY)
    {
        _action = REPA_UPDATE;
    }
}
