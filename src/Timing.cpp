//
//  Timing.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Timing.hpp"

#include "Constants.hpp"

Timing::Timing() :
_time(0)
{
    // Empty
}

void Timing::onFrame()
{
    _time += FRAME_RATE;
}

void Timing::reset()
{
    _time = 0;
}

float Timing::getTime() const
{
    return _time;
}
