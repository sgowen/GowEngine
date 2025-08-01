//
//  EntityControllerRender.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/17/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/common/Macros.hpp"

class Entity;

class EntityControllerRender
{
    DECL_RTTI_NOPARENT;
    DECL_EntityControllerRender_create;
    
public:
    EntityControllerRender(Entity* e);
    virtual ~EntityControllerRender() {}
    
    virtual std::string getTextureMapping();
    virtual std::string getTextureMapping(uint8_t state);
    virtual std::vector<SoundMapping>* getSoundMapping();
    virtual std::vector<SoundMapping>* getSoundMapping(uint8_t state);
    
protected:
    Entity* _entity;
};
