//
//  ReplicationManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ReplicationManagerClient::read(InputMemoryBitStream& imbs, EntityRegistry& er)
{
    static uint32_t minRequiredBitsForAction = 34;
    while (imbs.getRemainingBitCount() >= minRequiredBitsForAction)
    {
        uint32_t networkID;
        imbs.read(networkID);
        
        uint8_t action;
        imbs.readBits(action, 2);
        
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
    uint32_t fourCCKey;
    imbs.read(fourCCKey);
    
    bool isEntityNew = false;
    Entity* e = er.getEntityByID(networkID);
    if (e == nullptr)
    {
        EntityInstanceDef eid(networkID, fourCCKey, 0, 0, false);
        EntityDef& ed = ASSETS_MGR.getEntityDef(eid._key);
        e = Entity::createEntity(ed, eid);
        
        isEntityNew = true;
    }
    
    e->controllerNetwork()->read(imbs);
    
    if (isEntityNew)
    {
        er.registerEntity(e);
    }
}

void ReplicationManagerClient::readAndDoUpdateAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID)
{
    Entity* e = er.getEntityByID(networkID);
    assert(e != nullptr);
    
    e->controllerNetwork()->read(imbs);
}

void ReplicationManagerClient::readAndDoDestroyAction(InputMemoryBitStream& imbs, EntityRegistry& er, uint32_t networkID)
{
    Entity* e = er.getEntityByID(networkID);
    if (e != nullptr)
    {
        er.deregisterEntity(e);
    }
}
