//
//  EntityNetworkController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityNetworkController.hpp"

#include "Entity.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"

#include "MemoryBitStreamUtil.hpp"

IMPL_EntityNetworkController_create(EntityNetworkController);

EntityNetworkController::EntityNetworkController(Entity* e, bool isServer) :
_entity(e),
_isServer(isServer)
{
    // Empty
}

void EntityNetworkController::read(InputMemoryBitStream& ip)
{
    Entity& e = *_entity;
    
    bool stateBit;
    
    ip.read(stateBit);
    if (stateBit)
    {
        MemoryBitStreamUtil::read(ip, e._pose._velocity.x, e._pose._velocity.y);
        MemoryBitStreamUtil::read(ip, e._pose._position.x, e._pose._position.y);
        
        if (!e.isFixedRotation())
        {
            ip.read(e._pose._angle);
        }
        
        if (e._groundSensorFixture)
        {
            ip.read<uint8_t, 4>(e._pose._numGroundContacts);
        }
        
        ip.read(e._pose._isFacingLeft);
        
        e.updateBodyFromPose();
        
        e._poseNetworkCache = e._pose;
    }
    
    if (e._entityDef._stateSensitive)
    {
        ip.read(stateBit);
        if (stateBit)
        {
            ip.read(e._state._stateTime);
            ip.read(e._state._state);
            ip.read(e._state._stateFlags);
            
            e._stateNetworkCache = e._state;
        }
    }
}

uint16_t EntityNetworkController::write(OutputMemoryBitStream& op, uint16_t dirtyState)
{
    Entity& e = *_entity;
    
    uint16_t writtenState = 0;
    
    bool pose = dirtyState & Entity::ReadStateFlag_Pose;
    op.write(pose);
    if (pose)
    {
        MemoryBitStreamUtil::write(op, e._pose._velocity.x, e._pose._velocity.y);
        MemoryBitStreamUtil::write(op, e._pose._position.x, e._pose._position.y);
        
        if (!e.isFixedRotation())
        {
            op.write(e._pose._angle);
        }
        
        if (e._groundSensorFixture)
        {
            op.write<uint8_t, 4>(e._pose._numGroundContacts);
        }
        
        op.write(e._pose._isFacingLeft);
        
        writtenState |= Entity::ReadStateFlag_Pose;
    }
    
    if (e._entityDef._stateSensitive)
    {
        bool state = dirtyState & Entity::ReadStateFlag_State;
        op.write(state);
        if (state)
        {
            op.write(e._state._stateTime);
            op.write(e._state._state);
            op.write(e._state._stateFlags);
            
            writtenState |= Entity::ReadStateFlag_State;
        }
    }
    
    return writtenState;
}

void EntityNetworkController::recallNetworkCache()
{
    Entity& e = *_entity;
    
    e._pose = e._poseNetworkCache;
    e._state = e._stateNetworkCache;
    
    e.updateBodyFromPose();
}

uint16_t EntityNetworkController::getDirtyState()
{
    uint16_t ret = 0;
    
    Entity& e = *_entity;
    
    if (e._poseNetworkCache != e._pose)
    {
        e._poseNetworkCache = e._pose;
        ret |= Entity::ReadStateFlag_Pose;
    }
    
    if (e._stateNetworkCache != e._state)
    {
        e._stateNetworkCache = e._state;
        ret |= Entity::ReadStateFlag_State;
    }
    
    return ret;
}

bool EntityNetworkController::isServer()
{
    return _isServer;
}
