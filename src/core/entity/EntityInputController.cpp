//
//  EntityInputController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/01/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityInputController)

EntityInputController::EntityInputController(Entity* e) :
_entity(e)
{
    // Empty
}

EntityInputController::~EntityInputController()
{
    // Empty
}
