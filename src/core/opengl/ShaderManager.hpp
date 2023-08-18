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
    void loadData(std::vector<ShaderDescriptor>& sds);
    void loadIntoOpenGLAndFreeData(std::vector<ShaderDescriptor>& sds);
    void reset();
    Shader& shader(std::string name);
    bool isShaderLoaded(std::string name);
    
private:
    ShaderLoader _loader;
    std::map<std::string, Shader> _shaders;
};
