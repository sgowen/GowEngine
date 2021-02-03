//
//  ReplicationManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerClient.hpp"

#include "EntityManager.hpp"
#include "InputMemoryBitStream.hpp"

#include "EntityMapper.hpp"
#include "Entity.hpp"
#include "EntityLayoutMapper.hpp"
#include "ReplicationAction.hpp"
#include "EntityNetworkController.hpp"

#include <cassert>

ReplicationManagerClient::ReplicationManagerClient(EntityManager* entityManager) :
_entityManager(entityManager)
{
    // Empty
}

void ReplicationManagerClient::read(InputMemoryBitStream& inputStream)
{
    while (inputStream.getRemainingBitCount() >= 34)
    {
        uint32_t networkID;
        inputStream.read(networkID);
        
        uint8_t action;
        inputStream.read<uint8_t, 2>(action);
        
        switch(action)
        {
            case REPA_CREATE:
                readAndDoCreateAction(inputStream, networkID);
                break;
            case REPA_UPDATE:
                readAndDoUpdateAction(inputStream, networkID);
                break;
            case REPA_DESTROY:
                readAndDoDestroyAction(inputStream, networkID);
                break;
        }
    }
}

void ReplicationManagerClient::readAndDoCreateAction(InputMemoryBitStream& ip, uint32_t networkID)
{
    uint32_t fourCCName;
    ip.read(fourCCName);
    
    Entity* e = _entityManager->getEntityByID(networkID);
    if (e == NULL)
    {
        EntityInstanceDef eid(networkID, fourCCName);
        e = ENTITY_MAPPER.createEntity(&eid, false);
        
        _entityManager->registerEntity(e);
        
        assert(e->getEntityDef()._key == fourCCName);
    }
    
    e->getNetworkController()->read(ip);
}

void ReplicationManagerClient::readAndDoUpdateAction(InputMemoryBitStream& ip, uint32_t networkID)
{
    Entity* e = _entityManager->getEntityByID(networkID);
    assert(e != NULL);
    
    e->getNetworkController()->read(ip);
}

void ReplicationManagerClient::readAndDoDestroyAction(InputMemoryBitStream& ip, uint32_t networkID)
{
    Entity* e = _entityManager->getEntityByID(networkID);
    if (e != NULL)
    {
        _entityManager->deregisterEntity(e);
    }
}
