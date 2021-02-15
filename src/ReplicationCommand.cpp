//
//  ReplicationCommand.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationCommand.hpp"

ReplicationCommand::ReplicationCommand() :
_action(REPA_CREATE),
_dirtyState(0)
{
    // Empty
}

ReplicationCommand::ReplicationCommand(uint16_t initialDirtyState) :
_action(REPA_CREATE),
_dirtyState(initialDirtyState)
{
    // Empty
}

void ReplicationCommand::handleCreateAckd()
{
    if (_action == REPA_CREATE)
    {
        // if the create is ack'd, we can demote to just an update...
        _action = REPA_UPDATE;
    }
}

void ReplicationCommand::addDirtyState(uint16_t state)
{
    _dirtyState |= state;
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

uint16_t ReplicationCommand::getDirtyState() const
{
    return _dirtyState;
}

void ReplicationCommand::clearDirtyState(uint16_t stateToClear)
{
    _dirtyState = 0;
    
    if (_action == REPA_DESTROY)
    {
        _action = REPA_UPDATE;
    }
}
