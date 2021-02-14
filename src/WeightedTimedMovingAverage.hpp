//
//  WeightedTimedMovingAverage.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class TimeTracker;

class WeightedTimedMovingAverage
{
public:
    WeightedTimedMovingAverage(TimeTracker* tt, float duration = 5.0f);
    
    void updatePerSecond(float value);
    void update(float value);
    float getValue() const;
    
private:
    TimeTracker* _timeTracker;
    float _timeLastEntryMade;
    float _duration;
    float _value;
};
