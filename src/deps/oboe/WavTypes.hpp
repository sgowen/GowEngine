//
//  WavTypes.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/PlatformMacros.hpp"
#if IS_ANDROID

#include <stdint.h>

typedef uint32_t RiffID;
typedef int32_t RiffInt32;
typedef int16_t RiffInt16;

inline RiffID makeRiffID(char a, char b, char c, char d)
{
    return ((RiffID)d << 24) | ((RiffID)c << 16) | ((RiffID)b << 8) | (RiffID)a;
}

#endif /* IS_ANDROID */
