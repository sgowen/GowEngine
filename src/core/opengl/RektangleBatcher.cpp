//
//  RektangleBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "RektangleBatcher.hpp"

#include "Color.hpp"
#include "Rektangle.hpp"
#include "Shader.hpp"
#include "OpenGLWrapper.hpp"

RektangleBatcher::RektangleBatcher(uint32_t maxBatchSize, bool isFill) :
_maxBatchSize(maxBatchSize),
_isFill(isFill),
_vertexBuffer(0),
_indexBuffer(0)
{
    _vertices.reserve(_maxBatchSize * NUM_VERTICES_PER_RECTANGLE);
}

void RektangleBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _vertices.capacity());
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
    
    _vertices.emplace_back(left, bottom);
    _vertices.emplace_back(left, top);
    _vertices.emplace_back(right, top);
    _vertices.emplace_back(right, bottom);
}

void RektangleBatcher::end(Shader& s, mat4& matrix, const Color& c)
{
    if (_vertices.empty())
    {
        return;
    }
    
    int numQuads = (int)_vertices.size() / NUM_VERTICES_PER_RECTANGLE;
    
    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_Matrix", matrix);
    OGL.bindColor(s, "u_Color", c);
    
    if (_isFill)
    {
        OGL.drawIndexed(GL_TRIANGLES, _indexBuffer, numQuads);
    }
    else
    {
        for (uint32_t i = 0; i < numQuads; ++i)
        {
            OGL.drawIndexed(GL_LINE_STRIP, _indexBuffer, 1, i);
        }
    }
    
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}