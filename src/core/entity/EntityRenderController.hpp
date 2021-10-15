//
//  EntityRenderController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define DECL_EntityRenderController_create                  \
public:                                               \
    static EntityRenderController* create(Entity* e)

#define IMPL_EntityRenderController_create(type)            \
EntityRenderController* type::create(Entity* e)             \
{                                                     \
    return new type(e);                               \
}

#define IMPL_EntityRenderController_create_NOPARENT         \
IMPL_EntityRenderController_create(EntityRenderController)

#include "core/common/RTTI.hpp"

class Entity;

class EntityRenderController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityRenderController_create;
    
public:
    EntityRenderController(Entity* e);
    virtual ~EntityRenderController() {}
    
    virtual std::string getTextureMapping();
    virtual std::string getTextureMapping(uint8_t state, uint8_t stateFlags = 0);
    virtual std::string getSoundMapping(uint8_t state);
    
protected:
    Entity* _entity;
};
