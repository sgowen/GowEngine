//
//  ShaderLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

struct Shader;

class ShaderLoader
{
public:    
    void loadShader(Shader& s);
    void unloadShader(Shader& s);
};
