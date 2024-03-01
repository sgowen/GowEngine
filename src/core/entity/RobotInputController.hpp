//
//  RobotInputController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityInputController.hpp"

enum RobotInputStateFlags
{
    RISF_MOVING_RIGHT       = 1 << 0,
    RISF_MOVING_LEFT        = 1 << 1,
    RISF_JUMPING            = 1 << 2,
    RISF_EXECUTING_ATTACK   = 1 << 3
};

class RobotInputController : public EntityInputController
{
    DECL_RTTI;
    DECL_EntityInputController_create;
    
public:
    RobotInputController(Entity* e);
    virtual ~RobotInputController() {}
    
    virtual void processEvent(uint16_t& inputState, CursorEvent* e) override;
    virtual void processEvent(uint16_t& inputState, GamepadEvent* e) override;
    virtual void processEvent(uint16_t& inputState, KeyboardEvent* e) override;
    virtual void runAI(uint16_t& inputState) override;
};
