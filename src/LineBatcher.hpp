//
//  LineBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
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
    LineBatcher(int maxBatchSize);
    ~LineBatcher() {}

    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void begin();
    void addLine(Line &line);
    void addLine(float oX, float oY, float eX, float eY);
    void end(Shader& s, mat4& matrix, const Color& c);

private:
    int _maxBatchSize;
    std::vector<VERTEX_2D> _vertices;
    uint32_t _vertexBuffer;
};
