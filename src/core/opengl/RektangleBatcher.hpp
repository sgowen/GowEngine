//
//  RektangleBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <vector>
#include <stdint.h>

class Vector2;
struct Color;
struct Rektangle;
struct Shader;

class RektangleBatcher
{
public:
    RektangleBatcher(uint32_t maxBatchSize, bool isFill);

    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addRektangle(Rektangle& r);
    void addRektangle(float left, float bottom, float right, float top);
    void end(Shader& s, mat4& matrix, const Color& c);

private:
    uint32_t _maxBatchSize;
    bool _isFill;
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    std::vector<VERTEX_2D> _vertices;
};
