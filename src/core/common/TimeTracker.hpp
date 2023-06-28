//
//  TimeTracker.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/engine/EngineConfig.hpp"

#include <stdint.h>

struct TimeTracker
{
    uint32_t _time;
    
    TimeTracker() : _time(0)
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
    
    double realTime()
    {
        return realTime(_time);
    }
    
    double realTime(uint32_t time)
    {
        static double frameRate = ENGINE_CFG.frameRate();
        return time * frameRate;
    }
};
