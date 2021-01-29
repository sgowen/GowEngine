//
//  WavTypes.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/22/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

typedef unsigned int RiffID;
typedef int RiffInt32;
typedef short RiffInt16;

inline RiffID makeRiffID(char a, char b, char c, char d)
{
    return ((RiffID)d << 24) | ((RiffID)c << 16) | ((RiffID)b << 8) | (RiffID)a;
}
