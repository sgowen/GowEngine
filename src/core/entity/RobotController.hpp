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
