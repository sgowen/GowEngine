//
//  ShaderManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderLoader.hpp"
#include "Shader.hpp"

#include <map>
#include <string>

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    Shader& shader(std::string name);
    
private:
    ShaderLoader _loader;
    std::map<std::string, Shader> _shaders;
};
