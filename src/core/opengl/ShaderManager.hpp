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
    void loadShaders(std::vector<ShaderDescriptor>& sds);
    void unloadShaders(std::vector<ShaderDescriptor>& sds);
    Shader& shader(std::string name);
    std::map<std::string, Shader>& shaders();
    
private:
    ShaderLoader _loader;
    std::map<std::string, Shader> _shaders;
};
