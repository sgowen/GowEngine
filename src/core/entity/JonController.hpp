//
//  JonController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityController.hpp"

enum InputStateFlags
{
    ISF_MOVING_RIGHT       = 1 << 0,
    ISF_MOVING_LEFT        = 1 << 1,
    ISF_JUMPING            = 1 << 2,
    ISF_EXECUTING_ABILITY  = 1 << 3,
    ISF_WARMING_UP         = 1 << 4,
    ISF_EXECUTING_ATTACK   = 1 << 5,
    ISF_TRIGGERING_SPECIAL = 1 << 6
};

class JonController : public EntityController
{    
    DECL_RTTI;
    DECL_EntityController_create;
    
public:
    JonController(Entity* e);
    virtual ~JonController() {}
    
    virtual void processInput(uint16_t inputState) override;
    virtual void onUpdate(uint32_t numMovesProcessed) override;
    virtual void onMessage(uint16_t message) override;
    
    bool isReleasingShockwave();
    bool isVampire();
    uint16_t shockwaveStateTime();
    
private:
    enum State
    {
        STATE_IDLE = 0,
        STATE_RUNNING = 1,
        STATE_JUMPING = 2,
        STATE_FALLING = 3,
        STATE_DOUBLE_JUMPING = 4,
        STATE_LANDING = 5,
        STATE_DRILL_STOMPING = 6,
        STATE_LANDING_ROLL = 7,
        STATE_WARMING_UP = 8,
        STATE_PUNCHING = 9,
        STATE_DRILLING = 10,
        STATE_ATTEMPTING_TO_TRANSFORM = 11,
        STATE_TRANSFORMING = 12,
        STATE_CANCELING_ATTEMPT_TO_TRANSFORM = 13,
        STATE_VAMPIRE_IDLE = 14,
        STATE_VAMPIRE_RUNNING = 15,
        STATE_VAMPIRE_JUMPING = 16,
        STATE_VAMPIRE_DASHING = 17,
        STATE_VAMPIRE_DOUBLE_JUMPING = 18,
        STATE_VAMPIRE_GLIDING = 19,
        STATE_VAMPIRE_FALLING = 20,
        STATE_VAMPIRE_LANDING = 21,
        STATE_ATTEMPTING_TO_REVERT = 22,
        STATE_REVERTING = 23,
        STATE_CANCELING_ATTEMPT_TO_REVERT = 24,
        STATE_VAMPIRE_DASH_COMPLETED = 25,
        STATE_RABBIT_DEATH = 26,
        STATE_VAMPIRE_DEATH = 27
    };
    
    enum StateFallingFlags
    {
        STATE_FALLING_FLAG_CAN_JUMP_AGAIN = 1 << 0
    };
    
    uint16_t _shockwaveStartTime;
    uint16_t _shockwaveStateTime;
    bool _isReleasingShockwave;
    
    void processMovementInput(uint16_t inputState);
    void processJumpInput(uint16_t inputState);
    void processAttackInput(uint16_t inputState);
    void processAbilityInput(uint16_t inputState);
    void processWarmingUpInput(uint16_t inputState);
    void processSpecialInput(uint16_t inputState);
    
    bool isRabbit();
    
    bool isMovementInputAllowed();
    bool isJumpInputAllowed();
    bool isAttackInputAllowed();
    bool isAbilityInputAllowed();
    bool isWarmingUpInputAllowed();
    bool isSpecialInputAllowed();
    
    bool needsStateChangeForMovement();
    bool canCurrentStateBeInterruptedByFalling();
    bool canCurrentStateBeInterruptedByLanding();
    bool needsToResolveNewStateOnceAnimationEnds();
    
    void resolveNewState();
};
