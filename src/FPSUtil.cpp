//
//  FPSUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "FPSUtil.hpp"

FPSUtil& FPSUtil::getInstance()
{
    static FPSUtil ret = FPSUtil();
    return ret;
}

void FPSUtil::update(double deltaTime)
{
    _stateTime += deltaTime;
    ++_numFrames;
    
    if (_stateTime >= 1.0)
    {
        _fPS = _numFrames;
        
        _stateTime = 0;
        _numFrames = 0;
    }
}

int FPSUtil::getFPS() const
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
