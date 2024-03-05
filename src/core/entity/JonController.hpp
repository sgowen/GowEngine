//
//  JonController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityController.hpp"

class JonController : public EntityController
{    
    DECL_RTTI;
    DECL_EntityController_create;
    
public:
    JonController(Entity* e);
    virtual ~JonController() {}
    
    virtual void processInput(uint16_t inputState) override;
    virtual void onUpdate(uint32_t numMovesProcessed) override;
    virtual void onMessage(uint16_t message, Entity* fromEntity) override;
    
private:
    void processMovementInput(uint16_t inputState);
    void processJumpInput(uint16_t inputState);
    void processAttackInput(uint16_t inputState);
    void processAbilityInput(uint16_t inputState);
    void processWarmingUpInput(uint16_t inputState);
    void processSpecialInput(uint16_t inputState);
    
    bool isRabbit();
    bool isVampire();
    
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
