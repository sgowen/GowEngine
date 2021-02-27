//
//  ReplicationManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerClient.hpp"

#include "ReplicationCommand.hpp"
#include "EntityRegistry.hpp"
#include "InputMemoryBitStream.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "EntityLayout.hpp"
#include "EntityNetworkController.hpp"

#include <cassert>

void ReplicationManagerClient::read(InputMemoryBitStream& imbs, EntityRegistry& er)
{
    static uint32_t minRequiredBitsForAction = 34;
    while (imbs.getRemainingBitCount() >= minRequiredBitsForAction)
    {
        uint32_t networkID;
        imbs.read(networkID);
        
        uint8_t action;
        imbs.read<uint8_t, 2>(action);
        
        switch (action)
        {
            case REPA_CREATE:
                readAndDoCreateAction(imbs, er, networkID);
                break;
            case REPA_UPDATE:
                readAndDoUpdateAction(imbs, er, networkID);
                break;
            case REPA_DESTROY:
                readAndDoDestroyAction(imbs, er, networkID);
                break;
        }
    }
}

void ReplicationManagerClient::readAndDoCreateAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID)
{
    uint32_t fourCCName;
    imbs.read(fourCCName);
    
    bool isEntityNew = false;
    Entity* e = er.getEntityByID(networkID);
    if (e == NULL)
    {
        EntityInstanceDef eid(networkID, fourCCName, 0, 0, false);
        e = ENTITY_MGR.createEntity(eid);
        
        isEntityNew = true;
    }
    
    e->networkController()->read(imbs);
    
    if (isEntityNew)
    {
        er.registerEntity(e);
    }
}

void ReplicationManagerClient::readAndDoUpdateAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID)
{
    Entity* e = er.getEntityByID(networkID);
    assert(e != NULL);
    
    e->networkController()->read(imbs);
}

void ReplicationManagerClient::readAndDoDestroyAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID)
{
    Entity* e = er.getEntityByID(networkID);
    if (e != NULL)
    {
        er.deregisterEntity(e);
    }
}
