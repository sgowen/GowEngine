//
//  WeightedTimedMovingAverage.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WeightedTimedMovingAverage.hpp"

#include "TimeTracker.hpp"

#include "MathUtil.hpp"

#include <assert.h>

WeightedTimedMovingAverage::WeightedTimedMovingAverage(TimeTracker* timing, float duration) :
_timeTracker(timing),
_timeLastEntryMade(_timeTracker->_time),
_duration(duration),
_value(0.f)
{
    // Empty
}

void WeightedTimedMovingAverage::updatePerSecond(float value)
{
    if (value < 0)
    {
        return;
    }
    
    float time = _timeTracker->_time;
    float timeSinceLastEntry = CLAMP(time - _timeLastEntryMade, 0, 10);
    
    float valueOverTime = value / timeSinceLastEntry;
    
    // now update our value by whatever amount of the duration that was..
    float fractionOfDuration  = (timeSinceLastEntry / _duration);
    if (fractionOfDuration > 1.0f)
    {
        fractionOfDuration = 1.0f;
    }
    
    _value = _value * (1.0f - fractionOfDuration) + valueOverTime * fractionOfDuration;
    
    _timeLastEntryMade = time;
}

void WeightedTimedMovingAverage::update(float value)
{
    if (value < 0)
    {
        return;
    }
    
    float time = _timeTracker->_time;
    float timeSinceLastEntry = CLAMP(time - _timeLastEntryMade, 0, 10);
    
    // now update our value by whatever amount of the duration that was..
    float fractionOfDuration  = (timeSinceLastEntry / _duration);
    if (fractionOfDuration > 1.0f)
    {
        fractionOfDuration = 1.0f;
    }
    
    _value = _value * (1.0f - fractionOfDuration) + value * fractionOfDuration;
    
    _timeLastEntryMade = time;
}

float WeightedTimedMovingAverage::getValue() const
{
    return _value;
}
