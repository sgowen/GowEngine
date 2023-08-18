//
//  ShaderManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void ShaderManager::loadData(std::vector<ShaderDescriptor>& shaderDescriptors)
{
    for (auto& sd : shaderDescriptors)
    {
        _shaders.emplace(sd._name, Shader{sd});
        Shader& s = shader(sd._name);
        _loader.loadData(s);
    }
}

void ShaderManager::loadIntoOpenGLAndFreeData(std::vector<ShaderDescriptor>& shaderDescriptors)
{
    for (auto& sd : shaderDescriptors)
    {
        Shader& s = shader(sd._name);
        OGL.loadShader(s);
        _loader.freeData(s);
    }
}

void ShaderManager::reset()
{
    for (auto& pair : _shaders)
    {
        Shader& s = pair.second;
        if (s._glHandle > 0)
        {
            OGL.unloadShader(s);
        }
        if (s._vertexShaderFileData != nullptr)
        {
            _loader.freeData(s);
        }
    }
    _shaders.clear();
}

Shader& ShaderManager::shader(std::string name)
{
    auto q = _shaders.find(name);
    assert(q != _shaders.end());
    return q->second;
}

bool ShaderManager::isShaderLoaded(std::string name)
{
    auto q = _shaders.find(name);
    if (q != _shaders.end())
    {
        Shader& s = q->second;
        return s._glHandle > 0;
    }
    
    return false;
}
