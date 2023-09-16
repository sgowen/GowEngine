//
//  RektangleBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

RektangleBatcher::RektangleBatcher(uint32_t maxBatchSize, bool isFill) :
_maxBatchSize(maxBatchSize),
_isFill(isFill),
_vertexBuffer(0),
_indexBuffer(0)
{
    // Empty
}

void RektangleBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _maxBatchSize * NUM_VERTICES_PER_RECTANGLE);
    _indexBuffer = OGL.loadRektangleIndexBuffer(_maxBatchSize);
}

void RektangleBatcher::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void RektangleBatcher::begin()
{
    _vertices.clear();
}

void RektangleBatcher::addRektangle(Rektangle& r)
{
    float left = r.left();
    float bottom = r.bottom();
    float right = left + r._width;
    float top = bottom + r._height;

    addRektangle(left, bottom, right, top);
}

void RektangleBatcher::addRektangle(float left, float bottom, float right, float top)
{
    assert((_vertices.size() / NUM_VERTICES_PER_RECTANGLE) < _maxBatchSize);

    if (_isFill)
    {
        _vertices.emplace_back(left, bottom);
        _vertices.emplace_back(left, top);
        _vertices.emplace_back(right, top);
        _vertices.emplace_back(right, bottom);
    }
    else
    {
        _vertices.emplace_back(left, bottom);
        _vertices.emplace_back(left, top);
        
        _vertices.emplace_back(left, top);
        _vertices.emplace_back(right, top);
        
        _vertices.emplace_back(right, top);
        _vertices.emplace_back(right, bottom);
        
        _vertices.emplace_back(right, bottom);
        _vertices.emplace_back(left, bottom);
    }
}

void RektangleBatcher::end(Shader& s, mat4& matrix, Color& c)
{
    if (_vertices.empty())
    {
        return;
    }

    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_Matrix", matrix);
    OGL.bindColor(s, "u_Color", c);

    if (_isFill)
    {
        uint32_t numQuads = (uint32_t)_vertices.size() / NUM_VERTICES_PER_RECTANGLE;
        OGL.drawTrianglesIndexed(_indexBuffer, numQuads);
    }
    else
    {
        OGL.drawLines(0, (uint32_t)_vertices.size());
    }

    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
