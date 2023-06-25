//
//  TriangleBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

TriangleBatcher::TriangleBatcher(uint32_t maxBatchSize, bool isFill) :
_maxBatchSize(maxBatchSize),
_isFill(isFill),
_vertexBuffer(0)
{
    // Empty
}

void TriangleBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _maxBatchSize * NUM_VERTICES_PER_TRIANGLE);
}

void TriangleBatcher::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
}

void TriangleBatcher::begin()
{
    _vertices.clear();
}

void TriangleBatcher::addTriangle(Triangle& t)
{
    addTriangle(
                t.getPointA()._x, t.getPointA()._y,
                t.getPointB()._x, t.getPointB()._y,
                t.getPointC()._x, t.getPointC()._y
                );
}

void TriangleBatcher::addTriangle(float leftX, float leftY, float topX, float topY, float rightX, float rightY)
{
    assert((_vertices.size() / NUM_VERTICES_PER_TRIANGLE) < _maxBatchSize);
    
    if (_isFill)
    {
        _vertices.emplace_back(leftX, leftY);
        _vertices.emplace_back(topX, topY);
        _vertices.emplace_back(rightX, rightY);
    }
    else
    {
        _vertices.emplace_back(leftX, leftY);
        _vertices.emplace_back(topX, topY);
        
        _vertices.emplace_back(topX, topY);
        _vertices.emplace_back(rightX, rightY);
        
        _vertices.emplace_back(rightX, rightY);
        _vertices.emplace_back(leftX, leftY);
    }
}

void TriangleBatcher::end(Shader& s, mat4& matrix, Color& c)
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
        OGL.drawTriangles(0, (uint32_t)_vertices.size());
    }
    else
    {
        OGL.drawLines(0, (uint32_t)_vertices.size());
    }
    
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
