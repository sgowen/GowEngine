//
//  WeightedTimedMovingAverage.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class Timing;

class WeightedTimedMovingAverage
{
public:
    WeightedTimedMovingAverage(Timing* timing, float duration = 5.0f);
    
    void updatePerSecond(float value);
    void update(float value);
    float getValue() const;
    
private:
    Timing* _timing;
    float _timeLastEntryMade;
    float _value;
    float _duration;
};
