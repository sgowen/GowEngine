//
//  ScriptLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ScriptLoader::loadData(Script& s)
{
    std::string filePath = s._desc._filePath;
    
    if (ENGINE_CFG.logLua())
    {
        LOG("ScriptLoader::loadData %s", filePath.c_str());
    }
    
    assert(s._fileData == nullptr);
    
    s._fileData = new FileData(ASSET_HANDLER.loadAsset(filePath));
}

void ScriptLoader::freeData(Script& s)
{
    std::string filePath = s._desc._filePath;
    
    if (ENGINE_CFG.logLua())
    {
        LOG("ScriptLoader::freeData %s", filePath.c_str());
    }
    
    assert(s._fileData != nullptr);
    
    ASSET_HANDLER.unloadAsset(*s._fileData);
    
    delete s._fileData;
    s._fileData = nullptr;
}
