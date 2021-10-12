//
//  TimeTracker.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct TimeTracker
{
    float _frameRate;
    uint32_t _time;
    
    TimeTracker(float frameRate) :
    _frameRate(frameRate),
    _time(0)
    {
        // Empty
    }

    void onFrame()
    {
        ++_time;
    }

    void reset()
    {
        _time = 0;
    }
    
    float realTime()
    {
        return realTime(_time);
    }
    
    float realTime(uint32_t time)
    {
        return time * _frameRate;
    }
};
