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

struct Vector2;
struct Color;
struct Rektangle;
struct Shader;

class RektangleBatcher
{
public:
    RektangleBatcher(int maxBatchSize, bool isFill);

    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void begin();
    void addRektangle(Rektangle& r);
    void addRektangle(float left, float bottom, float right, float top);
    void end(Shader& s, mat4& matrix, const Color& c);

private:
    int _maxBatchSize;
    bool _isFill;
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    std::vector<VERTEX_2D> _vertices;
};
