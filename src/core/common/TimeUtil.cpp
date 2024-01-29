//
//  TimeUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/23/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

uint64_t TimeUtil::timeSinceEpochMillisec()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
