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
    
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string);
    
    if (ENGINE_CFG.logLua())
    {
        lua.set_function("LOG", [](std::string line) {
            LOG(line.c_str());
        });
    }
    
    lua.set_function("networkIsPlayerIDLocal", [](int playerID) {
        bool ret = false;
        if (NW_CLNT)
        {
            ret = NW_CLNT->isPlayerIDLocal(playerID);
        }
        return ret;
    });
    
    lua.set_function("networkAvgRoundTripTime", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->avgRoundTripTime());
        }
        return ret;
    });
    
    lua.set_function("networkBytesReceivedPerSecond", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->bytesReceivedPerSecond());
        }
        return ret;
    });
    
    lua.set_function("networkBytesSentPerSecond", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->bytesSentPerSecond());
        }
        return ret;
    });
    
    lua.set_function("fps", []() {
        return FPS_UTIL.fps();
    });
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
