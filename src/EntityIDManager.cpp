//
//  EntityIDManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "EntityIDManager.hpp"

#include "MathUtil.hpp"

#define  LAYOUT_ENTITY_ID_BEGIN 0x00000000u
#define  LAYOUT_ENTITY_ID_END   0x000FFFFFu
// 1048575 layout (loaded from json) entities supported, which should be MORE than enough

#define NETWORK_ENTITY_ID_BEGIN 0x000FFFFFu
#define NETWORK_ENTITY_ID_END   0x00FFFFFFu
// 15728639 dynamic entities loaded from a layout or via game logic supported

#define PLAYER_ENTITY_ID_BEGIN 0x00FFFFFFu
#define PLAYER_ENTITY_ID_END   0xFFFFFFFFu
// 4278190079 players supported - though if we go through the whole list... there could be repeats...
// but surely players will drop (causing a reset) before we reach this awfully high number!

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
