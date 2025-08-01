//
//  ScriptManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ScriptManager::loadData(std::map<std::string, FileDescriptor>& sds)
{
    for (auto& pair : sds)
    {
        _scripts.emplace(pair.first, Script{pair.second});
        Script& s = script(pair.first);
        _loader.loadData(s);
    }
}

void ScriptManager::loadIntoLuaAndFreeData(std::map<std::string, FileDescriptor>& sds)
{
    for (auto& pair : sds)
    {
        Script& s = script(pair.first);
        LUA.loadScript(s);
        _loader.freeData(s);
    }
}

void ScriptManager::reset()
{
    for (auto& pair : _scripts)
    {
        Script& s = pair.second;
        if (s._lua != nullptr)
        {
            LUA.unloadScript(s);
        }
        if (s._fileData != nullptr)
        {
            _loader.freeData(s);
        }
    }
    _scripts.clear();
}

Script& ScriptManager::script(std::string name)
{
    auto q = _scripts.find(name);
    assert(q != _scripts.end());
    return q->second;
}

bool ScriptManager::isScriptLoaded(std::string name)
{
    auto q = _scripts.find(name);
    if (q != _scripts.end())
    {
        Script& s = q->second;
        return s._lua != nullptr;
    }
    
    return false;
}
