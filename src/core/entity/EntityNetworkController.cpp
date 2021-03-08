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
#include "TopDownPhysicsController.hpp"
#include "core/audio/SoundUtil.hpp"
#include "NetworkClient.hpp"

IMPL_EntityController_create(EntityNetworkController)

EntityNetworkController::EntityNetworkController(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityNetworkController::read(InputMemoryBitStream& imbs)
{
    Entity& e = *_entity;
    uint8_t fromState = e.stateCache()._state;
    
    bool stateBit;
    
    imbs.read(stateBit);
    if (stateBit)
    {
        MemoryBitStreamUtil::read(imbs, e._pose._position._x, e._pose._position._y);
        MemoryBitStreamUtil::read(imbs, e._pose._velocity._x, e._pose._velocity._y);
        
        if (!e.isFixedRotation())
        {
            imbs.read(e._pose._angle);
        }
        
        if (!e.physicsController()->getRTTI().isDerivedFrom(TopDownPhysicsController::rtti))
        {
            // FIXME, this is a hacky way to determine that we are using the Box2D physics controller
            imbs.read<uint8_t, 4>(e._pose._numGroundContacts);
        }
        
        imbs.read(e._pose._isXFlipped);
        
        e._poseCache = e._pose;
    }
    
    imbs.read(stateBit);
    if (stateBit)
    {
        imbs.read(e._state._state);
        imbs.read(e._state._stateFlags);
        imbs.read(e._state._stateTime);
        
        e._stateCache = e._state;
    }
    
    NetworkData& nd = e.nwData();
    for (NetworkDataGroup& ndg : nd._data)
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            for (NetworkDataField& ndf : ndg._data)
            {
                MemoryBitStreamUtil::read(imbs, ndf);
            }
            
            ndg._dataCache = ndg._data;
        }
    }
    
    uint8_t playerID = _entity->data().getUInt("playerID");
    if (!NW_CLNT->isPlayerIDLocal(playerID))
    {
        SoundUtil::playSoundForStateIfChanged(e, fromState, e.state()._state);
    }
}

uint8_t EntityNetworkController::write(OutputMemoryBitStream& ombs, uint8_t dirtyState)
{
    Entity& e = *_entity;
    
    uint8_t ret = 0;
    
    bool pose = IS_BIT_SET(dirtyState, RSTF_POSE);
    ombs.write(pose);
    if (pose)
    {
        MemoryBitStreamUtil::write(ombs, e._pose._position._x, e._pose._position._y);
        MemoryBitStreamUtil::write(ombs, e._pose._velocity._x, e._pose._velocity._y);
        
        if (!e.isFixedRotation())
        {
            ombs.write(e._pose._angle);
        }
        
        if (!e.physicsController()->getRTTI().isDerivedFrom(TopDownPhysicsController::rtti))
        {
            // FIXME, this is a hacky way to determine that we are using the Box2D physics controller
            ombs.write<uint8_t, 4>(e._pose._numGroundContacts);
        }
        
        ombs.write(e._pose._isXFlipped);
        
        ret |= RSTF_POSE;
    }
    
    bool state = IS_BIT_SET(dirtyState, RSTF_STATE);
    ombs.write(state);
    if (state)
    {
        ombs.write(e._state._state);
        ombs.write(e._state._stateFlags);
        ombs.write(e._state._stateTime);
        
        ret |= RSTF_STATE;
    }
    
    NetworkData& nd = e.nwData();
    for (NetworkDataGroup& ndg : nd._data)
    {
        bool readStateFlag = IS_BIT_SET(dirtyState, ndg._readStateFlag);
        ombs.write(readStateFlag);
        if (readStateFlag)
        {
            for (NetworkDataField& ndf : ndg._data)
            {
                MemoryBitStreamUtil::write(ombs, ndf);
            }
            
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}

void EntityNetworkController::recallCache()
{
    Entity& e = *_entity;
    
    e._pose = e._poseCache;
    e._state = e._stateCache;
    
    NetworkData& nd = e.nwData();
    for (NetworkDataGroup& ndg : nd._data)
    {
        ndg._data = ndg._dataCache;
    }
    
    e.physicsController()->updateBodyFromPose();
}

uint8_t EntityNetworkController::refreshDirtyState()
{
    uint8_t ret = 0;
    
    Entity& e = *_entity;
    
    if (e._poseCache != e._pose)
    {
        e._poseCache = e._pose;
        ret |= RSTF_POSE;
    }
    
    if (e._stateCache != e._state)
    {
        e._stateCache = e._state;
        ret |= RSTF_STATE;
    }
    
    NetworkData& nd = e.nwData();
    for (NetworkDataGroup& ndg : nd._data)
    {
        if (ndg._dataCache != ndg._data)
        {
            ndg._dataCache = ndg._data;
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}
