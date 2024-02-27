//
//  RobotController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityController.hpp"

enum RobotInputStateFlags
{
    RISF_MOVING_RIGHT       = 1 << 0,
    RISF_MOVING_LEFT        = 1 << 1,
    RISF_JUMPING            = 1 << 2,
    RISF_EXECUTING_ATTACK   = 1 << 3
};

class RobotController : public EntityController
{    
    DECL_RTTI;
    DECL_EntityController_create;
    
public:
    RobotController(Entity* e);
    virtual ~RobotController() {}
    
    virtual void processInput(uint16_t inputState) override;
    virtual void onUpdate(uint32_t numMovesProcessed) override;
    virtual void onMessage(uint16_t message) override;
    
private:
    enum State
    {
        STATE_IDLE = 0,
        STATE_RUNNING = 1,
        STATE_JUMPING = 2
    };
    
    void processMovementInput(uint16_t inputState);
    void processJumpInput(uint16_t inputState);
    
    bool isMovementInputAllowed();
    bool isJumpInputAllowed();
    
    bool needsStateChangeForMovement();
    bool needsToResolveNewStateOnceAnimationEnds();
    bool canCurrentStateBeInterruptedByLanding();
    
    void resolveNewState();
};
