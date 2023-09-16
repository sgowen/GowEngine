//
//  MovingAverage.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct TimeTracker;

class MovingAverage
{
public:
    MovingAverage(TimeTracker& tt, float duration = 5.0f);
    
    void updatePerSecond(float value);
    void update(float value);
    float value() const;
    
private:
    TimeTracker& _timeTracker;
    float _timeLastEntryMade;
    float _duration;
    float _value;
};
