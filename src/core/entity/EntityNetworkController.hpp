//
//  EntityNetworkController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "Entity.hpp"

#include <stdint.h>
#include <map>

class InputMemoryBitStream;
class OutputMemoryBitStream;

class EntityNetworkController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityNetworkController_create;
    
public:
    EntityNetworkController(Entity* e);
    virtual ~EntityNetworkController() {}
    
    virtual void read(InputMemoryBitStream& imbs);
    virtual uint8_t write(OutputMemoryBitStream& ombs, uint8_t dirtyState);
    virtual void storeToCache(uint32_t numMovesProcessed);
    virtual void recallCache(uint32_t numMovesProcessed);
    virtual void clearCache(uint32_t numMovesProcessed);
    virtual uint8_t refreshDirtyState();
    
protected:
    Entity* _entity;
    
private:
    std::map<uint32_t, Entity::Pose> _poseCache;
    std::map<uint32_t, Entity::State> _stateCache;
    std::map<uint32_t, NetworkData> _networkDataCache;
};
