//
//  LuaUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/18/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

void LuaUtil::loadScript(Script& s)
{
    if (ENGINE_CFG.logLua())
    {
        LOG("LuaUtil::loadScript %s", s._desc._filePath.c_str());
    }
    
    assert(s._fileData != nullptr && s._fileData->_length > 0);
    
    s._lua = new sol::state();
    sol::state& lua = *s._lua;
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid())
    {
        LOG("LuaUtil: %s is not valid", s._desc._name.c_str());
    }
    assert(result.valid());
}

void LuaUtil::unloadScript(Script& s)
{
    if (ENGINE_CFG.logLua())
    {
        LOG("LuaUtil::unloadScript %s", s._desc._filePath.c_str());
    }
    
    assert(s._lua != nullptr);
    
    delete s._lua;
    s._lua = nullptr;
}
