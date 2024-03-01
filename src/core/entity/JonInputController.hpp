//
//  JonInputController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityInputController.hpp"

enum JonInputStateFlags
{
    JISF_MOVING_RIGHT       = 1 << 0,
    JISF_MOVING_LEFT        = 1 << 1,
    JISF_JUMPING            = 1 << 2,
    JISF_EXECUTING_ABILITY  = 1 << 3,
    JISF_WARMING_UP         = 1 << 4,
    JISF_EXECUTING_ATTACK   = 1 << 5,
    JISF_TRIGGERING_SPECIAL = 1 << 6
};

class JonInputController : public EntityInputController
{
    DECL_RTTI;
    DECL_EntityInputController_create;
    
public:
    JonInputController(Entity* e);
    virtual ~JonInputController() {}
    
    virtual void processEvent(uint16_t& inputState, CursorEvent* e) override;
    virtual void processEvent(uint16_t& inputState, GamepadEvent* e) override;
    virtual void processEvent(uint16_t& inputState, KeyboardEvent* e) override;
    virtual void runAI(uint16_t& inputState) override;
};
