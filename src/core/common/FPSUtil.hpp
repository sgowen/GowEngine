//
//  FPSUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define FPS_UTIL FPSUtil::getInstance()

class FPSUtil
{
public:
    static FPSUtil& getInstance()
    {
        static FPSUtil ret = FPSUtil();
        return ret;
    }
    
    void update(float deltaTime);
    int fps() const;
    
private:
    FPSUtil();
    ~FPSUtil() {}
    
    float _stateTime;
    int _fPS;
    int _numFrames;
};
