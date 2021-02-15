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
    AssetHandler* ah = AssetHandlerFactory::create();
    
    const FileData vertexShaderSrc = ah->loadAsset(s._descriptor._vertexShaderFilePath.c_str());
    const FileData fragmentShaderSrc = ah->loadAsset(s._descriptor._fragmentShaderFilePath.c_str());

    OGL.loadShader(s, vertexShaderSrc._data, vertexShaderSrc._length, fragmentShaderSrc._data, fragmentShaderSrc._length);

    ah->releaseAsset(vertexShaderSrc);
    ah->releaseAsset(fragmentShaderSrc);

    AssetHandlerFactory::destroy(ah);
}

void ShaderLoader::unloadShader(Shader& s)
{
    OGL.unloadShader(s);
}
