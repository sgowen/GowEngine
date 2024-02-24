//
//  RobotController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/24/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(RobotController, EntityController)
IMPL_EntityController_create(RobotController)

RobotController::RobotController(Entity* e) : EntityController(e)
{
    // Empty
}

void RobotController::processInput(uint16_t inputState)
{
    // TODO
}

void RobotController::onUpdate(uint32_t numMovesProcessed)
{
    // TODO
}

void RobotController::onMessage(uint16_t message)
{
    // TODO
}
