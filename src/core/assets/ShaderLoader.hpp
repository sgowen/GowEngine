//
//  ShaderLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

struct Shader;

class ShaderLoader
{
public:    
    void loadData(Shader& s);
    void freeData(Shader& s);
};
