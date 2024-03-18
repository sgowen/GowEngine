//
//  LuaUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/18/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

struct Script;

class LuaUtil
{
public:
    static LuaUtil& getInstance()
    {
        static LuaUtil ret = LuaUtil();
        return ret;
    }
    
    void loadScript(Script& s);
    void unloadScript(Script& s);
    
private:
    LuaUtil() {}
    ~LuaUtil() {}
    LuaUtil(const LuaUtil&);
    LuaUtil& operator=(const LuaUtil&);
};
