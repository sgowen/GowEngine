//
//  EntityNetworkController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"

#include <stdint.h>

class Entity;
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
    virtual void recallCache();
    virtual uint8_t refreshDirtyState();
    
protected:
    Entity* _entity;
};
