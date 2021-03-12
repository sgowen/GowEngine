//
//  ShaderLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ShaderLoader::loadShader(Shader& s)
{
    s._vertexShaderFileData = new FileData(ASSET_HANDLER.loadAsset(s._desc._vertexShaderFilePath));
    s._fragmentShaderFileData = new FileData(ASSET_HANDLER.loadAsset(s._desc._fragmentShaderFilePath));
}

void ShaderLoader::unloadShader(Shader& s)
{
    assert(s._vertexShaderFileData != NULL);
    assert(s._fragmentShaderFileData != NULL);
    
    ASSET_HANDLER.unloadAsset(*s._vertexShaderFileData);
    ASSET_HANDLER.unloadAsset(*s._fragmentShaderFileData);
}
