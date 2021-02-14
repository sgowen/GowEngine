//
//  ReplicationManagerServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerServer.hpp"

#include "EntityRegistry.hpp"
#include "OutputMemoryBitStream.hpp"
#include "ReplicationManagerTransmissionData.hpp"

#include "EntityRegistry.hpp"
#include "ReplicationAction.hpp"
#include "Macros.hpp"
#include "Entity.hpp"
#include "EntityNetworkController.hpp"

ReplicationManagerServer::ReplicationManagerServer(EntityRegistry& entityRegistry) :
_entityRegistry(entityRegistry)
{
    // Empty
}

void ReplicationManagerServer::replicateCreate(uint32_t networkID, uint16_t initialDirtyState)
{
    _networkIDToReplicationCommand.emplace(networkID, ReplicationCommand{initialDirtyState});
}

void ReplicationManagerServer::replicateDestroy(uint32_t networkID)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand[networkID].setDestroy();
}

void ReplicationManagerServer::removeFromReplication(uint32_t networkID)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand.erase(networkID);
}

void ReplicationManagerServer::setStateDirty(uint32_t networkID, uint16_t dirtyState)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand[networkID].addDirtyState(dirtyState);
}

void ReplicationManagerServer::handleCreateAckd(uint32_t networkID)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand[networkID].handleCreateAckd();
}

void ReplicationManagerServer::write(OutputMemoryBitStream& ombs, ReplicationManagerTransmissionData* ioTransmissdata)
{
    for (auto& pair: _networkIDToReplicationCommand)
    {
        ReplicationCommand& rc = pair.second;
        if (rc.hasDirtyState())
        {
            uint32_t networkID = pair.first;
            
            ombs.write(networkID);
            
            ReplicationAction ra = rc.getAction();
            ombs.write<uint8_t, 2>(static_cast<uint8_t>(ra));
            
            uint16_t writtenState = 0;
            uint16_t dirtyState = rc.getDirtyState();
            
            switch(ra)
            {
                case REPA_CREATE:
                    writtenState = writeCreateAction(ombs, networkID, dirtyState);
                    break;
                case REPA_UPDATE:
                    writtenState = writeUpdateAction(ombs, networkID, dirtyState);
                    break;
                case REPA_DESTROY:
                    writtenState = dirtyState;
                    break;
            }
            
            ioTransmissdata->addTransmission(networkID, ra, writtenState);
            
            rc.clearDirtyState(writtenState);
        }
    }
}

uint16_t ReplicationManagerServer::writeCreateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint16_t dirtyState)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    assert(e != NULL);
    
    ombs.write(e->entityDef()._key);
    
    return e->networkController()->write(ombs, dirtyState);
}

uint16_t ReplicationManagerServer::writeUpdateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint16_t dirtyState)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    assert(e != NULL);
    
    return e->networkController()->write(ombs, dirtyState);
}
