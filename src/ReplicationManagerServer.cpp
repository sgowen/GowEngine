//
//  ReplicationManagerServer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerServer.hpp"

#include "EntityManager.hpp"
#include "OutputMemoryBitStream.hpp"
#include "ReplicationManagerTransmissionData.hpp"

#include "EntityManager.hpp"
#include "ReplicationAction.hpp"
#include "Macros.hpp"
#include "Entity.hpp"
#include "EntityNetworkController.hpp"

ReplicationManagerServer::ReplicationManagerServer(EntityManager* entityManager) :
_entityManager(entityManager)
{
    // Empty
}

void ReplicationManagerServer::replicateCreate(uint32_t networkID, uint16_t initialDirtyState)
{
    _networkIDToReplicationCommand[networkID] = ReplicationCommand(initialDirtyState);
}

void ReplicationManagerServer::replicateDestroy(uint32_t networkID)
{
    //it's broken if we don't find it...
    _networkIDToReplicationCommand[networkID].setDestroy();
}

void ReplicationManagerServer::removeFromReplication(uint32_t networkID)
{
    _networkIDToReplicationCommand.erase(networkID);
}

void ReplicationManagerServer::setStateDirty(uint32_t networkID, uint16_t dirtyState)
{
    _networkIDToReplicationCommand[networkID].addDirtyState(dirtyState);
}

void ReplicationManagerServer::handleCreateAckd(uint32_t networkID)
{
    _networkIDToReplicationCommand[networkID].handleCreateAckd();
}

void ReplicationManagerServer::write(OutputMemoryBitStream& outputStream, ReplicationManagerTransmissionData* ioTransmissdata)
{
    //run through each replication command and do something...
    for (auto& pair: _networkIDToReplicationCommand)
    {
        ReplicationCommand& replicationCommand = pair.second;
        if (replicationCommand.hasDirtyState())
        {
            uint32_t networkID = pair.first;
            
            //well, first write the network id...
            outputStream.write(networkID);
            
            //only need 2 bits for action...
            ReplicationAction action = replicationCommand.getAction();
            outputStream.write<uint8_t, 2>(static_cast<uint8_t>(action));
            
            uint16_t writtenState = 0;
            uint16_t dirtyState = replicationCommand.getDirtyState();
            
            //now do what?
            switch(action)
            {
                case REPA_CREATE:
                    writtenState = writeCreateAction(outputStream, networkID, dirtyState);
                    break;
                case REPA_UPDATE:
                    writtenState = writeUpdateAction(outputStream, networkID, dirtyState);
                    break;
                case REPA_DESTROY:
                    //don't need anything other than state!
                    writtenState = writeDestroyAction(outputStream, networkID, dirtyState);
                    break;
            }
            
            ioTransmissdata->addTransmission(networkID, action, writtenState);
            
            //let's pretend everything was written- don't make this too hard
            replicationCommand.clearDirtyState(writtenState);
        }
    }
}

uint16_t ReplicationManagerServer::writeCreateAction(OutputMemoryBitStream& op, uint32_t networkID, uint16_t dirtyState)
{
    //need object
    Entity* e = _entityManager->getEntityByID(networkID);
    //need 4 cc
    op.write(e->getEntityDef()._key);
    
    return e->getNetworkController()->write(op, dirtyState);
}

uint16_t ReplicationManagerServer::writeUpdateAction(OutputMemoryBitStream& op, uint32_t networkID, uint16_t dirtyState)
{
    //need object
    Entity* e = _entityManager->getEntityByID(networkID);
    
    //if we can't find the entity on the other side, we won't be able to read the written data (since we won't know which class wrote it)
    //so we need to know how many bytes to skip.
    
    //this means we need byte sand each new object needs to be byte aligned
    
    return e->getNetworkController()->write(op, dirtyState);
}

uint16_t ReplicationManagerServer::writeDestroyAction(OutputMemoryBitStream& op, uint32_t networkID, uint16_t dirtyState)
{
    UNUSED(op);
    UNUSED(networkID);
    UNUSED(dirtyState);
    
    //don't have to do anything- action already written
    
    return dirtyState;
}
