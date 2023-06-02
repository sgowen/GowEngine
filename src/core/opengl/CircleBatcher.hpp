//
//  CircleBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <vector>
#include <stdint.h>

struct Circle;
struct Color;
struct Shader;

class CircleBatcher
{
public:
    CircleBatcher(uint32_t maxBatchSize);

    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addCircle(Circle& c);
    void addCircle(float x, float y, float radius);
    void addPartialCircle(Circle& c, int arcDegrees);
    void addPartialCircle(float x, float y, float radius, int arcDegrees);
    void end(Shader& s, mat4& matrix, Color& c);

private:
    uint32_t _maxBatchSize;
    std::vector<VERTEX_2D> _vertices;
    std::vector<size_t> _circles;
    uint32_t _vertexBuffer;
};
