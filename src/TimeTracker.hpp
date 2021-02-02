//
//  TimeTracker.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct TimeTracker
{
    double _frameRate;
    double _time;
    
    TimeTracker(double frameRate) :
    _frameRate(frameRate),
    _time(0)
    {
        // Empty
    }

    void onFrame()
    {
        _time += _frameRate;
    }

    void reset()
    {
        _time = 0;
    }
};
