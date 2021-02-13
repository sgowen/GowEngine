//
//  ReplicationManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerClient.hpp"

#include "EntityRegistry.hpp"
#include "InputMemoryBitStream.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "EntityLayoutManager.hpp"
#include "ReplicationAction.hpp"
#include "EntityNetworkController.hpp"

#include <cassert>

ReplicationManagerClient::ReplicationManagerClient(EntityRegistry& entityRegistry) :
_entityRegistry(entityRegistry)
{
    // Empty
}

void ReplicationManagerClient::read(InputMemoryBitStream& imbs)
{
    while (imbs.getRemainingBitCount() >= 34)
    {
        uint32_t networkID;
        imbs.read(networkID);
        
        uint8_t action;
        imbs.read<uint8_t, 2>(action);
        
        switch(action)
        {
            case REPA_CREATE:
                readAndDoCreateAction(imbs, networkID);
                break;
            case REPA_UPDATE:
                readAndDoUpdateAction(imbs, networkID);
                break;
            case REPA_DESTROY:
                readAndDoDestroyAction(imbs, networkID);
                break;
        }
    }
}

void ReplicationManagerClient::readAndDoCreateAction(InputMemoryBitStream& imbs, uint32_t networkID)
{
    uint32_t fourCCName;
    imbs.read(fourCCName);
    
    bool isEntityNew = false;
    Entity* e = _entityRegistry.getEntityByID(networkID);
    if (e == NULL)
    {
        EntityInstanceDef eid(networkID, fourCCName);
        e = ENTITY_MGR.createEntity(eid, false);
        
        isEntityNew = true;
    }
    
    e->networkController()->read(imbs);
    
    if (isEntityNew)
    {
        _entityRegistry.registerEntity(e);
    }
}

void ReplicationManagerClient::readAndDoUpdateAction(InputMemoryBitStream& imbs, uint32_t networkID)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    assert(e != NULL);
    
    e->networkController()->read(imbs);
}

void ReplicationManagerClient::readAndDoDestroyAction(InputMemoryBitStream& imbs, uint32_t networkID)
{
    Entity* e = _entityRegistry.getEntityByID(networkID);
    if (e != NULL)
    {
        _entityRegistry.deregisterEntity(e);
    }
}
