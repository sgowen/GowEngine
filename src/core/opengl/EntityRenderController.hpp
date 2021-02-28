//
//  EntityRenderController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "RTTI.hpp"
#include "Macros.hpp"

class Entity;
class SpriteBatcher;

class EntityRenderController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityController_create(EntityRenderController);
    
public:
    EntityRenderController(Entity* e);
    virtual ~EntityRenderController() {}
    
    virtual void addSprite(SpriteBatcher& sb);
    virtual std::string getTextureMapping();
    virtual std::string getTextureMapping(uint8_t state);
    virtual uint16_t getSoundMapping(uint8_t state);
    
protected:
    Entity* _entity;
};
