//
//  TriangleBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <vector>
#include <stdint.h>

struct Color;
struct Triangle;
struct Shader;

class TriangleBatcher
{
public:
    TriangleBatcher(uint32_t maxBatchSize, bool isFill);

    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addTriangle(Triangle& t);
    void addTriangle(float leftX, float leftY, float topX, float topY, float rightX, float rightY);
    void end(Shader& s, mat4& matrix, Color& c);

private:
    uint32_t _maxBatchSize;
    bool _isFill;
    uint32_t _vertexBuffer;
    std::vector<VERTEX_2D> _vertices;
};
