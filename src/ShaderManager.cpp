//
//  ShaderManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ShaderManager.hpp"

#include "ShaderDescriptor.hpp"
#include "Assets.hpp"

ShaderManager::ShaderManager()
{
    // Empty
}

ShaderManager::~ShaderManager()
{
    // Empty
}

void ShaderManager::createDeviceDependentResources()
{
    assert(_shaders.size() == 0);
    
    std::vector<ShaderDescriptor>& sds = ASSETS.getShaderDescriptors();
    for (auto& sd : sds)
    {
        _shaders.insert({sd._name, Shader(sd)});
    }
    
    for (std::map<std::string, Shader>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
    {
        _loader.loadShader(i->second);
    }
}

void ShaderManager::releaseDeviceDependentResources()
{
    for (std::map<std::string, Shader>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
    {
        _loader.unloadShader(i->second);
    }
    
    _shaders.clear();
}

Shader& ShaderManager::shader(std::string name)
{
    auto q = _shaders.find(name);
    
    assert(q != _shaders.end());
    
    return q->second;
}
