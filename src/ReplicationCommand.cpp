//
//  ReplicationCommand.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationCommand.hpp"

ReplicationCommand::ReplicationCommand()
{
    // Empty
}

ReplicationCommand::ReplicationCommand(uint16_t initialDirtyState) :
_action(RA_Create),
_dirtyState(initialDirtyState)
{
    // Empty
}

void ReplicationCommand::handleCreateAckd()
{
    if (_action == RA_Create)
    {
        // if the create is ack'd, we can demote to just an update...
        _action = RA_Update;
    }
}

void ReplicationCommand::addDirtyState(uint16_t state)
{
    _dirtyState |= state;
}

void ReplicationCommand::setDestroy()
{
    _action = RA_Destroy;
}

bool ReplicationCommand::hasDirtyState() const
{
    return (_action == RA_Destroy) || (_dirtyState != 0);
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
    _dirtyState &= ~stateToClear;
    
    if (_action == RA_Destroy)
    {
        _action = RA_Update;
    }
}
