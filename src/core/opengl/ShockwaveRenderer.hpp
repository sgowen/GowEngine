//
//  ShockwaveRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/7/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <stdint.h>

struct Shader;
class Framebuffer;
struct Matrix;

class ShockwaveRenderer
{
public:
    ShockwaveRenderer();
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void render(Shader& s, Framebuffer& fb, Matrix& m, float x, float y, float timeElapsed, bool isTransforming);
    
private:
    std::vector<VERTEX_2D> _vertices;
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
};
