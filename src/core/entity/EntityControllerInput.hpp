//
//  EntityControllerInput.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"
#include "core/common/Macros.hpp"

class Entity;
struct CursorEvent;
struct GamepadEvent;
struct KeyboardEvent;

class EntityControllerInput
{
    DECL_RTTI_NOPARENT;
    DECL_EntityControllerInput_create;
    
public:
    EntityControllerInput(Entity* e);
    virtual ~EntityControllerInput() {}
    
    virtual void processEvent(uint16_t& inputState, CursorEvent* ce);
    virtual void processEvent(uint16_t& inputState, GamepadEvent* gpe);
    virtual void processEvent(uint16_t& inputState, KeyboardEvent* kbe);
    
protected:
    Entity* _entity;
};
