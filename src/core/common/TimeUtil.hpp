//
//  TimeUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/23/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class TimeUtil
{
public:
    static uint64_t timeSinceEpochMillisec();
    
private:
    TimeUtil();
    ~TimeUtil() {}
    TimeUtil(const TimeUtil&);
    TimeUtil& operator=(const TimeUtil&);
};
