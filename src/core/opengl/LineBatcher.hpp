//
//  LineBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <vector>
#include <stdint.h>

struct Color;
struct Line;
struct Shader;

class LineBatcher
{
public:
    LineBatcher(uint32_t maxBatchSize);

    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addLine(Line &line);
    void addLine(float oX, float oY, float eX, float eY);
    void end(Shader& s, mat4& matrix, Color& c);

private:
    uint32_t _maxBatchSize;
    std::vector<VERTEX_2D> _vertices;
    uint32_t _vertexBuffer;
};
