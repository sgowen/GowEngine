//
//  ShaderManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ShaderManager.hpp"

#include "ShaderDescriptor.hpp"
#include "OpenGLWrapper.hpp"

void ShaderManager::loadShaders(std::vector<ShaderDescriptor>& sds)
{
    for (auto& sd : sds)
    {
        _shaders.emplace(sd._name, Shader{sd});
    }
    
    for (std::map<std::string, Shader>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
    {
        Shader& s = i->second;
        _loader.loadShader(s);
        OGL.loadShader(s);
        _loader.unloadShader(s);
    }
}

void ShaderManager::unloadShaders(std::vector<ShaderDescriptor>& sds)
{
    for (std::map<std::string, Shader>::iterator i = _shaders.begin(); i != _shaders.end(); ++i)
    {
        Shader& s = i->second;
        OGL.unloadShader(s);
    }
    
    _shaders.clear();
}

Shader& ShaderManager::shader(std::string name)
{
    auto q = _shaders.find(name);
    assert(q != _shaders.end());
    return q->second;
}

std::map<std::string, Shader>& ShaderManager::shaders()
{
    return _shaders;
}
