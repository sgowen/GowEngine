//
//  ReplicationCommand.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

enum ReplicationAction
{
    REPA_CREATE = 1,
    REPA_UPDATE,
    REPA_DESTROY
};

class OutputMemoryBitStream;
class InputMemoryBitStream;

class ReplicationCommand
{
public:
    ReplicationCommand();
    ReplicationCommand(uint8_t initialDirtyState);
    
    void handleCreateAckd();
    void addDirtyState(uint8_t dirtyState);
    void setDestroy();
    bool hasDirtyState() const;
    ReplicationAction getAction() const;
    uint8_t getDirtyState() const;
    void clearDirtyState();
    
private:
    ReplicationAction _action;
    uint8_t _dirtyState;
};
