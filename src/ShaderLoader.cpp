//
//  ShaderLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ShaderLoader.hpp"

#include "Shader.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "OpenGLWrapper.hpp"

void ShaderLoader::loadShader(Shader& s)
{
    const FileData vertexShader = ASSET_HANDLER.loadAsset(s._desc._vertexShaderFilePath.c_str());
    const FileData fragmentShader = ASSET_HANDLER.loadAsset(s._desc._fragmentShaderFilePath.c_str());

    OGL.loadShader(s, vertexShader._data, vertexShader._length, fragmentShader._data, fragmentShader._length);

    ASSET_HANDLER.unloadAsset(vertexShader);
    ASSET_HANDLER.unloadAsset(fragmentShader);
}

void ShaderLoader::unloadShader(Shader& s)
{
    OGL.unloadShader(s);
}
