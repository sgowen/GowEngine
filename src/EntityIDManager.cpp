//
//  EntityIDManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityIDManager.hpp"

#include "Constants.hpp"
#include "MathUtil.hpp"

EntityIDManager::EntityIDManager() :
_staticEntityID(STATIC_ENTITY_ID_BEGIN),
_dynamicEntityID(DYNAMIC_ENTITY_ID_BEGIN)
{
    // Empty
}

EntityIDManager::~EntityIDManager()
{
    // Empty
}

void EntityIDManager::reset()
{
    _staticEntityID = STATIC_ENTITY_ID_BEGIN;
}

uint32_t EntityIDManager::getNextStaticEntityID()
{
    _staticEntityID = clamp(_staticEntityID + 1, STATIC_ENTITY_ID_BEGIN, STATIC_ENTITY_ID_END);
    
    return _staticEntityID;
}

uint32_t EntityIDManager::getNextDynamicEntityID()
{
    _dynamicEntityID = clamp(_dynamicEntityID + 1, DYNAMIC_ENTITY_ID_BEGIN, DYNAMIC_ENTITY_ID_END);
    
    return _dynamicEntityID;
}
