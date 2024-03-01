//
//  MonsterInputController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityInputController.hpp"

enum MonsterInputStateFlags
{
    MISF_MOVING_RIGHT       = 1 << 0,
    MISF_MOVING_LEFT        = 1 << 1,
    MISF_EXECUTING_ATTACK   = 1 << 2
};

class MonsterInputController : public EntityInputController
{
    DECL_RTTI;
    DECL_EntityInputController_create;
    
public:
    MonsterInputController(Entity* e);
    virtual ~MonsterInputController() {}
    
    virtual void processEvent(uint16_t& inputState, CursorEvent* e) override;
    virtual void processEvent(uint16_t& inputState, GamepadEvent* e) override;
    virtual void processEvent(uint16_t& inputState, KeyboardEvent* e) override;
    virtual void runAI(uint16_t& inputState) override;
};
