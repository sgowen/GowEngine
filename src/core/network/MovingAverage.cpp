//
//  MovingAverage.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

MovingAverage::MovingAverage(TimeTracker* tt, float duration) :
_timeTracker(tt),
_timeLastEntryMade(tt->realTime()),
_duration(duration),
_value(0)
{
    // Empty
}

void MovingAverage::updatePerSecond(float value)
{
    if (value < 0)
    {
        return;
    }
    
    float time = _timeTracker->realTime();
    float timeSinceLastEntry = CLAMP(time - _timeLastEntryMade, 0, 10);
    float valueOverTime = value / timeSinceLastEntry;
    float fractionOfDuration  = CLAMP(timeSinceLastEntry / _duration, 0, 1);
    
    _value = _value * (1.0f - fractionOfDuration) + valueOverTime * fractionOfDuration;
    _timeLastEntryMade = time;
}

void MovingAverage::update(float value)
{
    if (value < 0)
    {
        return;
    }
    
    float time = _timeTracker->realTime();
    float timeSinceLastEntry = CLAMP(time - _timeLastEntryMade, 0, 10);
    float fractionOfDuration  = CLAMP(timeSinceLastEntry / _duration, 0, 1);
    
    _value = _value * (1.0f - fractionOfDuration) + value * fractionOfDuration;
    _timeLastEntryMade = time;
}

float MovingAverage::getValue() const
{
    return _value;
}
