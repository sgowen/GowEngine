//
//  ReplicationManagerServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ReplicationCommand.hpp"

#include <map>

class EntityRegistry;
class OutputMemoryBitStream;
class ReplicationTransmissionData;

class ReplicationManagerServer
{
public:
    ReplicationManagerServer(EntityRegistry& entityRegistry);
    
    void replicateCreate(uint32_t networkID);
    void replicateDestroy(uint32_t networkID);
    void setStateDirty(uint32_t networkID, uint8_t dirtyState);
    void handleCreateAckd(uint32_t networkID);
    void removeFromReplication(uint32_t networkID);
    void write(OutputMemoryBitStream& ombs, ReplicationTransmissionData* rtd);
    
private:
    EntityRegistry& _entityRegistry;
    std::map<uint32_t, ReplicationCommand> _networkIDToReplicationCommand;
    
    uint8_t writeCreateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint8_t dirtyState);
    uint8_t writeUpdateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint8_t dirtyState);
};
