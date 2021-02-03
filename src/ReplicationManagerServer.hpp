//
//  ReplicationManagerServer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Network.hpp"

#include "ReplicationCommand.hpp"

#include <map>

class EntityManager;
class OutputMemoryBitStream;
class ReplicationManagerTransmissionData;

class ReplicationManagerServer
{
public:
    ReplicationManagerServer(EntityManager* entityManager);
    
    void replicateCreate(uint32_t networkID, uint16_t initialDirtyState);
    void replicateDestroy(uint32_t networkID);
    void setStateDirty(uint32_t networkID, uint16_t dirtyState);
    void handleCreateAckd(uint32_t networkID);
    void removeFromReplication(uint32_t networkID);
    void write(OutputMemoryBitStream& outputStream, ReplicationManagerTransmissionData* ioTransmissdata);
    
private:
    EntityManager* _entityManager;
    std::map<uint32_t, ReplicationCommand> _networkIDToReplicationCommand;
    
    uint16_t writeCreateAction(OutputMemoryBitStream& outputStream, uint32_t networkID, uint16_t dirtyState);
    uint16_t writeUpdateAction(OutputMemoryBitStream& outputStream, uint32_t networkID, uint16_t dirtyState);
};
