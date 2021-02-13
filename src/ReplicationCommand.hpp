//
//  ReplicationCommand.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ReplicationAction.hpp"

#include <stdint.h>

class OutputMemoryBitStream;
class InputMemoryBitStream;

class ReplicationCommand
{
public:
    ReplicationCommand();
    ReplicationCommand(uint16_t initialDirtyState);
    
    void handleCreateAckd();
    void addDirtyState(uint16_t state);
    void setDestroy();
    bool hasDirtyState() const;
    ReplicationAction getAction() const;
    uint16_t getDirtyState() const;
    void clearDirtyState(uint16_t stateToClear);
    
private:
    ReplicationAction _action;
    uint16_t _dirtyState;
};
