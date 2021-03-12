//
//  EntityController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/5/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityController)
IMPL_EntityController_NOPARENT_create(EntityController)

EntityController::EntityController(Entity* e) :
_entity(e)
{
    // Empty
}
