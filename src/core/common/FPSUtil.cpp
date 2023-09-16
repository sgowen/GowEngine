//
//  FPSUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void FPSUtil::update(double deltaTime)
{
    _stateTime += deltaTime;
}

void FPSUtil::onFrame()
{
    ++_numFrames;
    
    if (_stateTime >= 1.0)
    {
        _fPS = _numFrames;
        
        _stateTime -= 1.0;
        _numFrames = 0;
    }
}

int FPSUtil::fps() const
{
    return _fPS;
}

FPSUtil::FPSUtil() :
_stateTime(0),
_fPS(0),
_numFrames(0)
{
    // Empty
}
