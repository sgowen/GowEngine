//
//  OpenALUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/11/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class OpenALUtil
{
public:
    static OpenALUtil& getInstance()
    {
        static OpenALUtil ret = OpenALUtil();
        return ret;
    }

    OpenALUtil() {}
    ~OpenALUtil() {}
    OpenALUtil(const OpenALUtil&);
    OpenALUtil& operator=(const OpenALUtil&);
};
