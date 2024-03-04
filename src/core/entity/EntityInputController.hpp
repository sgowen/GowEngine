//
//  EntityInputController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/RTTI.hpp"

class Entity;
struct CursorEvent;
struct GamepadEvent;
struct KeyboardEvent;

class EntityInputController
{
    DECL_RTTI_NOPARENT;
    DECL_EntityInputController_create;
    
public:
    EntityInputController(Entity* e);
    virtual ~EntityInputController() {}
    
    virtual void processEvent(uint16_t& inputState, CursorEvent* ce);
    virtual void processEvent(uint16_t& inputState, GamepadEvent* gpe);
    virtual void processEvent(uint16_t& inputState, KeyboardEvent* kbe);
    
protected:
    Entity* _entity;
};
