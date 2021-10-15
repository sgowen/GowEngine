//
//  EntityIDManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define  LAYOUT_ENTITY_ID_BEGIN 0x00000000u
#define  LAYOUT_ENTITY_ID_END   0x000FFFFFu

#define NETWORK_ENTITY_ID_BEGIN LAYOUT_ENTITY_ID_END
#define NETWORK_ENTITY_ID_END   0x00FFFFFFu

#define PLAYER_ENTITY_ID_BEGIN  NETWORK_ENTITY_ID_END
#define PLAYER_ENTITY_ID_END    0xFFFFFFFFu

EntityIDManager::EntityIDManager() :
_nextLayoutEntityID(LAYOUT_ENTITY_ID_BEGIN),
_nextNetworkEntityID(NETWORK_ENTITY_ID_BEGIN),
_nextPlayerEntityID(PLAYER_ENTITY_ID_BEGIN)
{
    // Empty
}

uint32_t EntityIDManager::getNextLayoutEntityID()
{
    _nextLayoutEntityID = CLAMP(_nextLayoutEntityID + 1, LAYOUT_ENTITY_ID_BEGIN, LAYOUT_ENTITY_ID_END);
    
    return _nextLayoutEntityID;
}

void EntityIDManager::resetNextLayoutEntityID()
{
    _nextLayoutEntityID = LAYOUT_ENTITY_ID_BEGIN;
}

uint32_t EntityIDManager::getNextNetworkEntityID()
{
    _nextNetworkEntityID = CLAMP(_nextNetworkEntityID + 1, NETWORK_ENTITY_ID_BEGIN, NETWORK_ENTITY_ID_END);
    
    return _nextNetworkEntityID;
}

void EntityIDManager::resetNextNetworkEntityID()
{
    _nextNetworkEntityID = NETWORK_ENTITY_ID_BEGIN;
}

uint32_t EntityIDManager::getNextPlayerEntityID()
{
    _nextPlayerEntityID = CLAMP(_nextPlayerEntityID + 1, PLAYER_ENTITY_ID_BEGIN, PLAYER_ENTITY_ID_END);
    
    return _nextPlayerEntityID;
}

void EntityIDManager::resetNextPlayerEntityID()
{
    _nextPlayerEntityID = PLAYER_ENTITY_ID_BEGIN;
}
