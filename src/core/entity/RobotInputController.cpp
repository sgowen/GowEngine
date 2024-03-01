//
//  RobotInputController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI(RobotInputController, EntityInputController)
IMPL_EntityInputController_create(RobotInputController)

RobotInputController::RobotInputController(Entity* e) : EntityInputController(e)
{
    // Empty
}

void RobotInputController::processEvent(uint16_t& inputState, CursorEvent* e)
{
    Matrix* m = INPUT_MGR.matrix();
    float halfWidth = m->_desc.width() / 2;
    float halfHeight = m->_desc.height() / 2;
    
    Vector2& pos = INPUT_MGR.convert(e);
    
    SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed() && pos._x < halfWidth);
    SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed() && pos._x > halfWidth);
    
    SET_BIT(inputState, RISF_JUMPING, e->isPressed() && pos._y > halfHeight);
}

void RobotInputController::processEvent(uint16_t& inputState, GamepadEvent* e)
{
    switch (e->_button)
    {
        case GPEB_BUTTON_A:
            SET_BIT(inputState, RISF_EXECUTING_ATTACK, e->isPressed());
            break;
        case GPEB_BUTTON_B:
            SET_BIT(inputState, RISF_JUMPING, e->isPressed());
            break;
        case GPEB_D_PAD_LEFT:
        {
            SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed());
            break;
        }
        case GPEB_D_PAD_RIGHT:
        {
            SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed());
            break;
        }
        case GPEB_STICK_LEFT:
        {
            SET_BIT(inputState, RISF_MOVING_LEFT, e->_x < 0);
            SET_BIT(inputState, RISF_MOVING_RIGHT, e->_x > 0);
            break;
        }
        default:
            break;
    }
}

void RobotInputController::processEvent(uint16_t& inputState, KeyboardEvent* e)
{
    switch (e->_key)
    {
        case GOW_KEY_J:
            SET_BIT(inputState, RISF_JUMPING, e->isPressed());
            break;
        case GOW_KEY_K:
            SET_BIT(inputState, RISF_EXECUTING_ATTACK, e->isPressed());
            break;
        case GOW_KEY_A:
            SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed());
            break;
        case GOW_KEY_D:
            SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed());
            break;
        case GOW_KEY_ARROW_UP:
            SET_BIT(inputState, RISF_JUMPING, e->isPressed());
            break;
        case GOW_KEY_ARROW_LEFT:
            SET_BIT(inputState, RISF_MOVING_LEFT, e->isPressed());
            break;
        case GOW_KEY_ARROW_RIGHT:
            SET_BIT(inputState, RISF_MOVING_RIGHT, e->isPressed());
            break;
        case GOW_KEY_ARROW_DOWN:
            SET_BIT(inputState, RISF_EXECUTING_ATTACK, e->isPressed());
            break;
        default:
            break;
    }
}

void RobotInputController::runAI(uint16_t& inputState)
{
    // Empty
}
