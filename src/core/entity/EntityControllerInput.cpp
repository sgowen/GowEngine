//
//  EntityControllerInput.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityControllerInput)
IMPL_EntityControllerInput_create_NOPARENT

EntityControllerInput::EntityControllerInput(Entity* e) :
_entity(e)
{
    // Empty
}

void EntityControllerInput::processEvent(uint16_t& inputState, CursorEvent* ce)
{
    // TODO, this code is temporary, and should be moved to JSON
    Entity& e = *_entity;
    Matrix* m = INPUT_MGR.matrix();
    float halfWidth = m->_desc.width() / 2;
    float halfHeight = m->_desc.height() / 2;
    
    Vector2& pos = INPUT_MGR.convert(ce);
    
    SET_BIT(inputState, e.inputStateFlag("MOVING_LEFT"), ce->isPressed() && pos._x < halfWidth);
    SET_BIT(inputState, e.inputStateFlag("MOVING_RIGHT"), ce->isPressed() && pos._x > halfWidth);
    
    SET_BIT(inputState, e.inputStateFlag("JUMPING"), ce->isPressed() && pos._y > halfHeight);
}

bool isConditionSatisfied(GamepadEvent* gpe, std::string condition)
{
    if (condition == "pressed")
    {
        return gpe->isPressed();
    }
    else if (condition == "down")
    {
        return gpe->isDown();
    }
    else if (condition == "held")
    {
        return gpe->isHeld();
    }
    else if (condition == "up")
    {
        return gpe->isUp();
    }
    else if (condition == "negativeX")
    {
        return gpe->_x < 0;
    }
    else if (condition == "positiveX")
    {
        return gpe->_x > 0;
    }
    
    return false;
}

void EntityControllerInput::processEvent(uint16_t& inputState, GamepadEvent* gpe)
{
    Entity& e = *_entity;
    
    EntityInputMapping& eim = ENTITY_INPUT_MAPPING_MGR.entityInputMapping(e._entityDef._inputMapping);
    
    std::string buttonMapping = INPUT_MGR.mappingForButton(gpe->_button);
    
    auto q = eim._gamepadEvents.find(buttonMapping);
    if (q == eim._gamepadEvents.end())
    {
        return;
    }
    
    std::vector<EntityInputMappingInstance>& entityInputMappingInstances = q->second;
    
    for (EntityInputMappingInstance& eimi : entityInputMappingInstances)
    {
        bool isBitOn = isConditionSatisfied(gpe, eimi._condition);
        SET_BIT(inputState, e.inputStateFlag(eimi._inputStateFlag), isBitOn);
    }
}

bool isConditionSatisfied(KeyboardEvent* kbe, std::string condition)
{
    if (condition == "pressed")
    {
        return kbe->isPressed();
    }
    else if (condition == "down")
    {
        return kbe->isDown();
    }
    else if (condition == "held")
    {
        return kbe->isHeld();
    }
    else if (condition == "up")
    {
        return kbe->isUp();
    }
    
    return false;
}

void EntityControllerInput::processEvent(uint16_t& inputState, KeyboardEvent* kbe)
{
    Entity& e = *_entity;
    
    EntityInputMapping& eim = ENTITY_INPUT_MAPPING_MGR.entityInputMapping(e._entityDef._inputMapping);
    
    std::string keyMapping = INPUT_MGR.mappingForKey(kbe->_key);
    
    auto q = eim._keyboardEvents.find(keyMapping);
    if (q == eim._keyboardEvents.end())
    {
        return;
    }
    
    std::vector<EntityInputMappingInstance>& entityInputMappingInstances = q->second;
    
    for (EntityInputMappingInstance& eimi : entityInputMappingInstances)
    {
        bool isBitOn = isConditionSatisfied(kbe, eimi._condition);
        SET_BIT(inputState, e.inputStateFlag(eimi._inputStateFlag), isBitOn);
    }
}
