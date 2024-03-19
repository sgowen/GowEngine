//
//  ShaderManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ShaderLoader.hpp"
#include "Shader.hpp"

#include <map>
#include <string>

class ShaderManager
{
public:
    void loadData(std::map<std::string, ShaderDescriptor>& sds);
    void loadIntoOpenGLAndFreeData(std::map<std::string, ShaderDescriptor>& sds);
    void reset();
    Shader& shader(std::string name);
    bool isShaderLoaded(std::string name);
    
private:
    ShaderLoader _loader;
    std::map<std::string, Shader> _shaders;
};
