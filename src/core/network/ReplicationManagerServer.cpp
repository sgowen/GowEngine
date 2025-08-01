//
//  ReplicationManagerServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ReplicationManagerServer::ReplicationManagerServer(EntityRegistry& entityRegistry) :
_entityRegistry(entityRegistry)
{
    // Empty
}

void ReplicationManagerServer::replicateCreate(uint32_t networkID)
{
    _networkIDToReplicationCommand.emplace(networkID, ReplicationCommand{NW_ALL_DIRTY_STATE});
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

void ReplicationManagerServer::setStateDirty(uint32_t networkID, uint8_t dirtyState)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand[networkID].addDirtyState(dirtyState);
}

void ReplicationManagerServer::handleCreateAckd(uint32_t networkID)
{
    assert(_networkIDToReplicationCommand.find(networkID) != _networkIDToReplicationCommand.end());
    
    _networkIDToReplicationCommand[networkID].handleCreateAckd();
}

void ReplicationManagerServer::write(OutputMemoryBitStream& ombs, ReplicationTransmissionData* rtd)
{
    for (auto& pair: _networkIDToReplicationCommand)
    {
        ReplicationCommand& rc = pair.second;
        if (!rc.hasDirtyState())
        {
            continue;
        }
        
        uint32_t networkID = pair.first;
        ombs.write(networkID);
        
        ReplicationAction ra = rc.getAction();
        ombs.writeBits(static_cast<uint8_t>(ra), 2);
        
        uint8_t writtenState = 0;
        uint8_t dirtyState = rc.getDirtyState();
        
        switch (ra)
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
        
        rtd->addTransmission(networkID, ra, writtenState);
        
        rc.clearDirtyState();
    }
}

uint8_t ReplicationManagerServer::writeCreateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint8_t dirtyState)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    assert(e != nullptr);
    
    EntityDef& ed = ASSETS_MGR.getEntityDef(e->entityDef()._key);
    ombs.write(ed._key);
    
    return e->controllerNetwork()->write(ombs, dirtyState);
}

uint8_t ReplicationManagerServer::writeUpdateAction(OutputMemoryBitStream& ombs, uint32_t networkID, uint8_t dirtyState)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    assert(e != nullptr);
    
    return e->controllerNetwork()->write(ombs, dirtyState);
}
