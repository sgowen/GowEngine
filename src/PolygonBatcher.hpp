//
//  PolygonBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <vector>
#include <stdint.h>

struct b2Vec2;
struct Color;
struct Rektangle;
struct Triangle;
struct Shader;

class PolygonBatcher
{
public:
    PolygonBatcher(int maxBatchSize, bool isFill);
    ~PolygonBatcher() {}

    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void begin();
    void addRektangle(Rektangle& r);
    void addRektangle(float left, float bottom, float right, float top);
    void addTriangle(Triangle& t);
    void addTriangle(float leftX, float leftY, float topX, float topY, float rightX, float rightY);
    void addPolygon(const b2Vec2* vertices, int vertexCount);
    void end(Shader& s, mat4& matrix, const Color& c);

private:
    int _maxBatchSize;
    bool _isFill;
    uint32_t _quadVertexBuffer;
    uint32_t _quadIndexBuffer;
    uint32_t _triangleVertexBuffer;
    std::vector<VERTEX_2D> _quadVertices;
    std::vector<VERTEX_2D> _triangleVertices;
};
