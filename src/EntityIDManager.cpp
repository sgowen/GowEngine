//
//  EntityIDManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityIDManager.hpp"

#include "MathUtil.hpp"

#define  LAYOUT_ENTITY_ID_BEGIN 0x00000001u
#define  LAYOUT_ENTITY_ID_END   0x000FFFFFu // 1048575 layout (loaded from json) entities supported, which should be MORE than enough
#define NETWORK_ENTITY_ID_BEGIN 0x00100000u
#define NETWORK_ENTITY_ID_END   0xFFFFFFFFu

EntityIDManager::EntityIDManager() :
_nextLayoutEntityID(LAYOUT_ENTITY_ID_BEGIN),
_nextNetworkEntityID(NETWORK_ENTITY_ID_BEGIN)
{
    // Empty
}

uint32_t EntityIDManager::getNextLayoutEntityID()
{
    _nextLayoutEntityID = clamp(_nextLayoutEntityID + 1, LAYOUT_ENTITY_ID_BEGIN, LAYOUT_ENTITY_ID_END);
    
    return _nextLayoutEntityID;
}

void EntityIDManager::resetLayoutEntityID()
{
    _nextLayoutEntityID = LAYOUT_ENTITY_ID_BEGIN;
}

uint32_t EntityIDManager::getNextNetworkEntityID()
{
    _nextNetworkEntityID = clamp(_nextNetworkEntityID + 1, NETWORK_ENTITY_ID_BEGIN, NETWORK_ENTITY_ID_END);
    
    return _nextNetworkEntityID;
}
