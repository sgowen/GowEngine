//
//  EntityControllerNetwork.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityControllerNetwork)
IMPL_EntityControllerNetwork_create_NOPARENT

EntityControllerNetwork::EntityControllerNetwork(Entity* e) :
_entity(e)
{
    storeToCache(0);
}

void EntityControllerNetwork::read(InputMemoryBitStream& imbs)
{
    const uint32_t numMovesProcessed = NW_CLNT->getNumMovesProcessed();
    
    Entity& e = *_entity;
    
    bool stateBit;
    
    imbs.read(stateBit);
    if (stateBit)
    {
        _poseCache.insert({numMovesProcessed, Entity::Pose(0, 0)});
        Entity::Pose& pose = _poseCache.at(numMovesProcessed);
        
        MemoryBitStreamUtil::read(imbs, pose._position._x, pose._position._y);
        MemoryBitStreamUtil::read(imbs, pose._velocity._x, pose._velocity._y);
        
        imbs.readBits(pose._numGroundContacts, 4);
        imbs.read(pose._isXFlipped);
        imbs.read(pose._isZeroGravity);
        imbs.read(pose._isSlowed);
    }
    
    imbs.read(stateBit);
    if (stateBit)
    {
        _stateCache.insert({numMovesProcessed, Entity::State()});
        Entity::State& state = _stateCache.at(numMovesProcessed);
        
        imbs.read(state._state);
        imbs.read(state._stateFlags);
        imbs.read(state._stateTime);
        
        state._lastProcessedInputState = 0;
        imbs.read(stateBit);
        if (stateBit)
        {
            imbs.read(state._lastProcessedInputState);
        }
    }
    
    if (e.isPlayer())
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            _playerInfoCache.insert({numMovesProcessed, Entity::PlayerInfo()});
            Entity::PlayerInfo& playerInfo = _playerInfoCache.at(numMovesProcessed);
            
            imbs.read(playerInfo._playerAddressHash);
            imbs.readSmall(playerInfo._playerAddress);
            imbs.readSmall(playerInfo._playerName);
            imbs.read(playerInfo._playerID);
        }
    }
    
    NetworkData& nd = e.networkData();
    size_t numDataGroups = nd._data.size();
    for (size_t i = 0; i < numDataGroups; ++i)
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            auto it = _networkDataCache.find(numMovesProcessed);
            if (it == _networkDataCache.end())
            {
                // TODO, may need to fix this up
                // So we assign the latest client-side version of the networkData here
                // And then read in the server update on top of it
                // but unless all NetworkDataFields have an update to be read in,
                // this frame of networkDataCache is going to contain parts that are potentially more up-to-date
                // than it should be for the given numMovesProcessed
                _networkDataCache.insert({numMovesProcessed, nd});
            }
            
            _networkDataCache.at(numMovesProcessed)._data[i] = nd._data[i];
            NetworkDataGroup& ndg = _networkDataCache.at(numMovesProcessed)._data[i];
            for (NetworkDataField& ndf : ndg._data)
            {
                MemoryBitStreamUtil::read(imbs, ndf);
            }
        }
    }
}

uint8_t EntityControllerNetwork::write(OutputMemoryBitStream& ombs, uint8_t dirtyState)
{
    Entity& e = *_entity;
    
    uint8_t ret = 0;
    
    bool pose = IS_BIT_SET(dirtyState, RSTF_POSE);
    ombs.write(pose);
    if (pose)
    {
        MemoryBitStreamUtil::write(ombs, e._pose._position._x, e._pose._position._y);
        MemoryBitStreamUtil::write(ombs, e._pose._velocity._x, e._pose._velocity._y);
        
        ombs.writeBits(e._pose._numGroundContacts, 4);
        ombs.write(e._pose._isXFlipped);
        ombs.write(e._pose._isZeroGravity);
        ombs.write(e._pose._isSlowed);
        
        ret |= RSTF_POSE;
    }
    
    bool state = IS_BIT_SET(dirtyState, RSTF_STATE);
    ombs.write(state);
    if (state)
    {
        ombs.write(e._state._state);
        ombs.write(e._state._stateFlags);
        ombs.write(e._state._stateTime);
        uint16_t inputState = e._state._lastProcessedInputState;
        bool write = inputState > 0;
        ombs.write(write);
        if (write)
        {
            ombs.write(inputState);
        }
        
        ret |= RSTF_STATE;
    }
    
    if (e.isPlayer())
    {
        bool playerInfo = IS_BIT_SET(dirtyState, RSTF_PLAYER_INFO);
        ombs.write(playerInfo);
        if (playerInfo)
        {
            ombs.write(e._playerInfo._playerAddressHash);
            ombs.writeSmall(e._playerInfo._playerAddress);
            ombs.writeSmall(e._playerInfo._playerName);
            ombs.write(e._playerInfo._playerID);
            
            ret |= RSTF_PLAYER_INFO;
        }
    }
    
    NetworkData& nd = e.networkData();
    for (NetworkDataGroup& ndg : nd._data)
    {
        bool readStateFlag = IS_BIT_SET(dirtyState, ndg._readStateFlag);
        ombs.write(readStateFlag);
        if (readStateFlag)
        {
            for (NetworkDataField& ndf : ndg._data)
            {
                MemoryBitStreamUtil::write(ombs, ndf);
            }
            
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}

void EntityControllerNetwork::storeToCache(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    
    if (_poseCache.find(numMovesProcessed) == _poseCache.end())
    {
        _poseCache.insert({numMovesProcessed, e._pose});
    }
    
    if (_stateCache.find(numMovesProcessed) == _stateCache.end())
    {
        _stateCache.insert({numMovesProcessed, e._state});
    }
    
    if (_playerInfoCache.find(numMovesProcessed) == _playerInfoCache.end())
    {
        _playerInfoCache.insert({numMovesProcessed, e._playerInfo});
    }
    
    if (_networkDataCache.find(numMovesProcessed) == _networkDataCache.end())
    {
        _networkDataCache.insert({numMovesProcessed, e._entityDef._networkData});
    }
}

void EntityControllerNetwork::recallCache(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    
    auto it_poseCache = _poseCache.find(numMovesProcessed);
    if (it_poseCache != _poseCache.end())
    {
        e._pose = it_poseCache->second;
    }
    else
    {
        e._pose = _poseCache.at(0);
    }
    
    auto it_stateCache = _stateCache.find(numMovesProcessed);
    if (it_stateCache != _stateCache.end())
    {
        e._state = it_stateCache->second;
    }
    else
    {
        e._state = _stateCache.at(0);
    }
    
    auto it_playerInfoCache = _playerInfoCache.find(numMovesProcessed);
    if (it_playerInfoCache != _playerInfoCache.end())
    {
        e._playerInfo = it_playerInfoCache->second;
    }
    else
    {
        e._playerInfo = _playerInfoCache.at(0);
    }
    
    auto it_networkDataCache = _networkDataCache.find(numMovesProcessed);
    if (it_networkDataCache != _networkDataCache.end())
    {
        e._entityDef._networkData = it_networkDataCache->second;
    }
    else
    {
        e._entityDef._networkData = _networkDataCache.at(0);
    }
}

void EntityControllerNetwork::clearCache(uint32_t numMovesProcessed)
{
    for (auto i = _poseCache.begin(); i != _poseCache.end(); )
    {
        if (i->first > 0 && i->first < numMovesProcessed)
        {
            i = _poseCache.erase(i);
        }
        else
        {
            ++i;
        }
    }
    
    for (auto i = _stateCache.begin(); i != _stateCache.end(); )
    {
        if (i->first > 0 && i->first < numMovesProcessed)
        {
            i = _stateCache.erase(i);
        }
        else
        {
            ++i;
        }
    }
    
    for (auto i = _playerInfoCache.begin(); i != _playerInfoCache.end(); )
    {
        if (i->first > 0 && i->first < numMovesProcessed)
        {
            i = _playerInfoCache.erase(i);
        }
        else
        {
            ++i;
        }
    }
    
    for (auto i = _networkDataCache.begin(); i != _networkDataCache.end(); )
    {
        if (i->first > 0 && i->first < numMovesProcessed)
        {
            i = _networkDataCache.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

uint8_t EntityControllerNetwork::refreshDirtyState()
{
    uint8_t ret = 0;
    
    Entity& e = *_entity;
    
    if (_poseCache.at(0) != e._pose)
    {
        _poseCache.insert({0, e._pose});
        ret |= RSTF_POSE;
    }
    
    if (_stateCache.at(0) != e._state)
    {
        _stateCache.insert({0, e._state});
        ret |= RSTF_STATE;
    }
    
    if (_playerInfoCache.at(0) != e._playerInfo)
    {
        _playerInfoCache.insert({0, e._playerInfo});
        ret |= RSTF_PLAYER_INFO;
    }
    
    NetworkData& nd = e.networkData();
    size_t numDataGroups = nd._data.size();
    for (size_t i = 0; i < numDataGroups; ++i)
    {
        NetworkDataGroup& ndg = nd._data[i];
        NetworkDataGroup& ndgCache = _networkDataCache.at(0)._data[i];
        if (ndgCache._data != ndg._data)
        {
            ndgCache._data = ndg._data;
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}
