//
//  ShaderLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ShaderLoader::loadData(Shader& s)
{
    std::string vertexShaderFilePath = s._desc._vertexShaderFilePath;
    std::string fragmentShaderFilePath = s._desc._fragmentShaderFilePath;
    
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("ShaderLoader::loadData %s %s", vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str());
    }
    
    assert(s._vertexShaderFileData == nullptr);
    assert(s._fragmentShaderFileData == nullptr);
    
    s._vertexShaderFileData = new FileData(ASSET_HANDLER.loadAsset(vertexShaderFilePath));
    s._fragmentShaderFileData = new FileData(ASSET_HANDLER.loadAsset(fragmentShaderFilePath));
}

void ShaderLoader::freeData(Shader& s)
{
    std::string vertexShaderFilePath = s._desc._vertexShaderFilePath;
    std::string fragmentShaderFilePath = s._desc._fragmentShaderFilePath;
    
    if (ENGINE_CFG.fileLoggingEnabled())
    {
        LOG("ShaderLoader::freeData %s %s", vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str());
    }
    
    assert(s._vertexShaderFileData != nullptr);
    assert(s._fragmentShaderFileData != nullptr);
    
    ASSET_HANDLER.unloadAsset(*s._vertexShaderFileData);
    ASSET_HANDLER.unloadAsset(*s._fragmentShaderFileData);
    
    delete s._vertexShaderFileData;
    s._vertexShaderFileData = nullptr;
    
    delete s._fragmentShaderFileData;
    s._fragmentShaderFileData = nullptr;
}
