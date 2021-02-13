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
#include "Macros.hpp"
#include "EntityPhysicsController.hpp"

IMPL_EntityNetworkController_create(EntityNetworkController)

EntityNetworkController::EntityNetworkController(Entity* e, bool isServer) :
_entity(e),
_isServer(isServer)
{
    // Empty
}

void EntityNetworkController::read(InputMemoryBitStream& imbs)
{
    Entity& e = *_entity;
    
    bool stateBit;
    
    imbs.read(stateBit);
    if (stateBit)
    {
        MemoryBitStreamUtil::read(imbs, e._pose._velocity._x, e._pose._velocity._y);
        MemoryBitStreamUtil::read(imbs, e._pose._position._x, e._pose._position._y);
        
        imbs.read(e._pose._width);
        imbs.read(e._pose._height);
        
        if (!IS_BIT_SET(e._entityDef._bodyFlags, BODF_FIXED_ROTATION))
        {
            imbs.read(e._pose._angle);
        }
        
        imbs.read<uint8_t, 4>(e._pose._numGroundContacts);
        
        imbs.read(e._pose._isFacingLeft);
        
        e.physicsController()->updateBodyFromPose();
        
        e._poseCache = e._pose;
    }
    
    if (IS_BIT_SET(e._entityDef._bodyFlags, BODF_DYNAMIC))
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            imbs.read(e._state._stateTime);
            imbs.read(e._state._state);
            imbs.read(e._state._stateFlags);
            
            e._stateCache = e._state;
        }
    }
}

uint16_t EntityNetworkController::write(OutputMemoryBitStream& ombs, uint16_t dirtyState)
{
    Entity& e = *_entity;
    
    uint16_t writtenState = 0;
    
    bool pose = IS_BIT_SET(dirtyState, Entity::RSTF_POSE);
    ombs.write(pose);
    if (pose)
    {
        MemoryBitStreamUtil::write(ombs, e._pose._velocity._x, e._pose._velocity._y);
        MemoryBitStreamUtil::write(ombs, e._pose._position._x, e._pose._position._y);
        
        ombs.write(e._pose._width);
        ombs.write(e._pose._height);
        
        if (!IS_BIT_SET(e._entityDef._bodyFlags, BODF_FIXED_ROTATION))
        {
            ombs.write(e._pose._angle);
        }
        
        ombs.write<uint8_t, 4>(e._pose._numGroundContacts);
        
        ombs.write(e._pose._isFacingLeft);
        
        writtenState |= Entity::RSTF_POSE;
    }
    
    if (IS_BIT_SET(e._entityDef._bodyFlags, BODF_DYNAMIC))
    {
        bool state = IS_BIT_SET(dirtyState, Entity::RSTF_STATE);
        ombs.write(state);
        if (state)
        {
            ombs.write(e._state._stateTime);
            ombs.write(e._state._state);
            ombs.write(e._state._stateFlags);
            
            writtenState |= Entity::RSTF_STATE;
        }
    }
    
    return writtenState;
}

void EntityNetworkController::recallCache()
{
    Entity& e = *_entity;
    
    e._pose = e._poseCache;
    e._state = e._stateCache;
    
    e.physicsController()->updateBodyFromPose();
}

uint16_t EntityNetworkController::refreshDirtyState()
{
    uint16_t ret = 0;
    
    Entity& e = *_entity;
    
    if (e._poseCache != e._pose)
    {
        e._poseCache = e._pose;
        ret |= Entity::RSTF_POSE;
    }
    
    if (e._stateCache != e._state)
    {
        e._stateCache = e._state;
        ret |= Entity::RSTF_STATE;
    }
    
    return ret;
}

bool EntityNetworkController::isServer()
{
    return _isServer;
}
