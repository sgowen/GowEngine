//
//  EntityNetworkController.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/17/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

IMPL_RTTI_NOPARENT(EntityNetworkController)
IMPL_EntityNetworkController_create_NOPARENT

EntityNetworkController::EntityNetworkController(Entity* e) :
_entity(e)
{
    _poseCache[0] = e->_pose;
    _stateCache[0] = e->_state;
    _networkDataCache[0] = e->data();
}

void EntityNetworkController::read(InputMemoryBitStream& imbs)
{
    const uint32_t numMovesProcessed = NW_CLNT->getNumMovesProcessed();
    
    Entity& e = *_entity;
    
    bool stateBit;
    
    imbs.read(stateBit);
    if (stateBit)
    {
        _poseCache[numMovesProcessed] = Entity::Pose(0, 0);
        Entity::Pose& pose = _poseCache[numMovesProcessed];
        
        MemoryBitStreamUtil::read(imbs, pose._position._x, pose._position._y);
        MemoryBitStreamUtil::read(imbs, pose._velocity._x, pose._velocity._y);
        
        imbs.readBits(pose._numGroundContacts, 4);
        imbs.read(pose._isXFlipped);
    }
    
    imbs.read(stateBit);
    if (stateBit)
    {
        _stateCache[numMovesProcessed] = Entity::State();
        Entity::State& state = _stateCache[numMovesProcessed];
        
        imbs.read(state._state);
        imbs.read(state._stateFlags);
        imbs.read(state._stateTime);
    }
    
    NetworkData& nd = e.data();
    size_t numDataGroups = nd._data.size();
    for (size_t i = 0; i < numDataGroups; ++i)
    {
        imbs.read(stateBit);
        if (stateBit)
        {
            auto it = _networkDataCache.find(numMovesProcessed);
            if (it == _networkDataCache.end())
            {
                _networkDataCache[numMovesProcessed] = nd;
            }
            
            _networkDataCache[numMovesProcessed]._data[i] = nd._data[i];
            NetworkDataGroup& ndg = _networkDataCache[numMovesProcessed]._data[i];
            for (NetworkDataField& ndf : ndg._data)
            {
                MemoryBitStreamUtil::read(imbs, ndf);
            }
        }
    }
}

uint8_t EntityNetworkController::write(OutputMemoryBitStream& ombs, uint8_t dirtyState)
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
        
        ret |= RSTF_POSE;
    }
    
    bool state = IS_BIT_SET(dirtyState, RSTF_STATE);
    ombs.write(state);
    if (state)
    {
        ombs.write(e._state._state);
        ombs.write(e._state._stateFlags);
        ombs.write(e._state._stateTime);
        
        ret |= RSTF_STATE;
    }
    
    NetworkData& nd = e.data();
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

void EntityNetworkController::storeToCache(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    
    _poseCache[numMovesProcessed] = e._pose;
    _stateCache[numMovesProcessed] = e._state;
    _networkDataCache[numMovesProcessed] = e._entityDef._networkData;
}

void EntityNetworkController::recallCache(uint32_t numMovesProcessed)
{
    Entity& e = *_entity;
    
    // TODO, need to incorporate a cache system that
    // can store up to maxNumMoves
    // Could be a vector?
    // Or just a raw ass array like what I'm using for my new sound frame system
    // For reading back, simply apply the modulus operator with maxNumMoves
    // Move
//    static uint8_t maxNumFramesOfRollback = ENGINE_CFG.maxNumFramesOfRollback();
//    static uint8_t numFramesOfInputDelay = ENGINE_CFG.numFramesOfInputDelay();
//    static uint8_t maxNumMoves = numFramesOfInputDelay + maxNumFramesOfRollback;
    
    // Maybe the size should be numFramesOfInputDelay instead.
    // Because if there is NO input delay, there's no need to cache anything but the latest update from the server
    // Ahhhh, but the lag could be so great that it takes until we are well into our
    // rollback allotted moves before we get an update.
    // We could be on frame 7, getting an update for frame 0, with an input delay of 5.
    // So the cache really does have to be "maxNumMoves" in size
    // For the typical 7, 5 set up, this means we are caching the last 12 server updates
//    auto it_poseCache = _poseCache.find(numMovesProcessed);
//    if (it_poseCache != _poseCache.end())
//    {
//        e._pose = it_poseCache->second;
//    }
    e._pose = _poseCache[numMovesProcessed];
    
//    auto it_stateCache = _stateCache.find(numMovesProcessed);
//    if (it_stateCache != _stateCache.end())
//    {
//        e._state = it_stateCache->second;
//    }
    e._state = _stateCache[numMovesProcessed];
    
//    auto it_networkDataCache = _networkDataCache.find(numMovesProcessed);
//    if (it_networkDataCache != _networkDataCache.end())
//    {
//        e._entityDef._networkData = it_networkDataCache->second;
//    }
    e._entityDef._networkData = _networkDataCache[numMovesProcessed];
}

uint8_t EntityNetworkController::refreshDirtyState()
{
    uint8_t ret = 0;
    
    Entity& e = *_entity;
    
    if (_poseCache[0] != e._pose)
    {
        _poseCache[0] = e._pose;
        ret |= RSTF_POSE;
    }
    
    if (_stateCache[0] != e._state)
    {
        _stateCache[0] = e._state;
        ret |= RSTF_STATE;
    }
    
    NetworkData& nd = e.data();
    size_t numDataGroups = nd._data.size();
    for (size_t i = 0; i < numDataGroups; ++i)
    {
        NetworkDataGroup& ndg = nd._data[i];
        NetworkDataGroup& ndgCache = _networkDataCache[0]._data[i];
        if (ndgCache._data != ndg._data)
        {
            ndgCache._data = ndg._data;
            ret |= ndg._readStateFlag;
        }
    }
    
    return ret;
}
