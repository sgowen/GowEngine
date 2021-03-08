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
    
    NetworkData& nd = e.entityDef()._networkData;
    for (NetworkDataGroup ndg : nd._data)
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            for (NetworkDataField ndf : ndg._data)
            {
                switch (ndf._type)
                {
                    case NDFT_BOOL:
                        imbs.read(ndf.valueBool());
                        break;
                    case NDFT_UINT8:
                        imbs.read(ndf.valueUInt8());
                        break;
                    case NDFT_UINT16:
                        imbs.read(ndf.valueUInt16());
                        break;
                    case NDFT_UINT32:
                        imbs.read(ndf.valueUInt32());
                        break;
                    case NDFT_UINT64:
                        imbs.read(ndf.valueUInt64());
                        break;
                    case NDFT_INT8:
                        imbs.read(ndf.valueInt8());
                        break;
                    case NDFT_INT16:
                        imbs.read(ndf.valueInt16());
                        break;
                    case NDFT_INT32:
                        imbs.read(ndf.valueInt32());
                        break;
                    case NDFT_INT64:
                        imbs.read(ndf.valueInt64());
                        break;
                    case NDFT_FLOAT:
                        imbs.read(ndf.valueFloat());
                        break;
                    case NDFT_DOUBLE:
                        imbs.read(ndf.valueDouble());
                        break;
                    case NDFT_SMALL_STRING:
                        imbs.readSmall(ndf.valueString());
                        break;
                    case NDFT_LARGE_STRING:
                        imbs.readLarge(ndf.valueString());
                        break;
                    default:
                        break;
                }
            }
            
            ndg._dataCache = ndg._data;
        }
    }
    
    uint8_t playerID = _entity->entityDef()._data.getUInt("playerID");
    if (!NW_CLNT->isPlayerIDLocal(playerID))
    {
        SoundUtil::playSoundForStateIfChanged(e, fromState, e.state()._state);
    }
}

uint8_t EntityNetworkController::write(OutputMemoryBitStream& ombs, uint8_t dirtyState)
{
    Entity& e = *_entity;
    
    uint8_t ret = 0;
    
    bool pose = IS_BIT_SET(dirtyState, Entity::RSTF_POSE);
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
        
        ret |= Entity::RSTF_POSE;
    }
    
    bool state = IS_BIT_SET(dirtyState, Entity::RSTF_STATE);
    ombs.write(state);
    if (state)
    {
        ombs.write(e._state._state);
        ombs.write(e._state._stateFlags);
        ombs.write(e._state._stateTime);
        
        ret |= Entity::RSTF_STATE;
    }
    
    NetworkData& nd = e.entityDef()._networkData;
    for (NetworkDataGroup ndg : nd._data)
    {
        bool readStateFlag = IS_BIT_SET(dirtyState, ndg._readStateFlag);
        ombs.write(readStateFlag);
        if (readStateFlag)
        {
            for (NetworkDataField ndf : ndg._data)
            {
                switch (ndf._type)
                {
                    case NDFT_BOOL:
                        ombs.write(ndf.valueBool());
                        break;
                    case NDFT_UINT8:
                        ombs.write(ndf.valueUInt8());
                        break;
                    case NDFT_UINT16:
                        ombs.write(ndf.valueUInt16());
                        break;
                    case NDFT_UINT32:
                        ombs.write(ndf.valueUInt32());
                        break;
                    case NDFT_UINT64:
                        ombs.write(ndf.valueUInt64());
                        break;
                    case NDFT_INT8:
                        ombs.write(ndf.valueInt8());
                        break;
                    case NDFT_INT16:
                        ombs.write(ndf.valueInt16());
                        break;
                    case NDFT_INT32:
                        ombs.write(ndf.valueInt32());
                        break;
                    case NDFT_INT64:
                        ombs.write(ndf.valueInt64());
                        break;
                    case NDFT_FLOAT:
                        ombs.write(ndf.valueFloat());
                        break;
                    case NDFT_DOUBLE:
                        ombs.write(ndf.valueDouble());
                        break;
                    case NDFT_SMALL_STRING:
                        ombs.writeSmall(ndf.valueString());
                        break;
                    case NDFT_LARGE_STRING:
                        ombs.writeLarge(ndf.valueString());
                        break;
                    default:
                        break;
                }
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
    
    NetworkData& nd = e.entityDef()._networkData;
    for (NetworkDataGroup ndg : nd._data)
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
        ret |= Entity::RSTF_POSE;
    }
    
    if (e._stateCache != e._state)
    {
        e._stateCache = e._state;
        ret |= Entity::RSTF_STATE;
    }
    
    NetworkData& nd = e.entityDef()._networkData;
    for (NetworkDataGroup ndg : nd._data)
    {
        if (ndg._dataCache != ndg._data)
        {
            ndg._dataCache = ndg._data;
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}
