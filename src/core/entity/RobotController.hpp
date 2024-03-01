//
//  RobotController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityController.hpp"

class RobotController : public EntityController
{    
    DECL_RTTI;
    DECL_EntityController_create;
    
public:
    RobotController(Entity* e);
    virtual ~RobotController() {}
    
    virtual void processInput(uint16_t inputState) override;
    virtual void onUpdate(uint32_t numMovesProcessed) override;
    virtual void onMessage(uint16_t message, Entity* fromEntity) override;
    
private:
    enum State
    {
        STATE_IDLE = 0,
        STATE_RUNNING = 1,
        STATE_JUMPING = 2,
        STATE_PUNCH_1 = 3,
        STATE_PUNCH_2 = 4,
        STATE_PUNCH_3 = 5
    };
    
    void processMovementInput(uint16_t inputState);
    void processJumpInput(uint16_t inputState);
    void processAttackInput(uint16_t inputState);
    
    bool isMovementInputAllowed();
    bool isJumpInputAllowed();
    bool isAttackInputAllowed();
    
    bool needsStateChangeForMovement();
    bool needsToResolveNewStateOnceAnimationEnds();
    
    void resolveNewState();
};
