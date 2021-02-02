//
//  EntityIDManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityIDManager.hpp"

#include "MathUtil.hpp"

#define  STATIC_ENTITY_ID_BEGIN 0x00000001u
#define  STATIC_ENTITY_ID_END   0x000FFFFFu // 1048575 static (loaded from json) entities supported, which should be MORE than enough
#define DYNAMIC_ENTITY_ID_BEGIN 0x00100000u
#define DYNAMIC_ENTITY_ID_END   0xFFFFFFFFu

EntityIDManager::EntityIDManager() :
_staticEntityID(STATIC_ENTITY_ID_BEGIN),
_dynamicEntityID(DYNAMIC_ENTITY_ID_BEGIN)
{
    // Empty
}

uint32_t EntityIDManager::getNextStaticEntityID()
{
    _staticEntityID = clamp(_staticEntityID + 1, STATIC_ENTITY_ID_BEGIN, STATIC_ENTITY_ID_END);
    
    return _staticEntityID;
}

void EntityIDManager::resetStaticEntityID()
{
    _staticEntityID = STATIC_ENTITY_ID_BEGIN;
}

uint32_t EntityIDManager::getNextDynamicEntityID()
{
    _dynamicEntityID = clamp(_dynamicEntityID + 1, DYNAMIC_ENTITY_ID_BEGIN, DYNAMIC_ENTITY_ID_END);
    
    return _dynamicEntityID;
}
