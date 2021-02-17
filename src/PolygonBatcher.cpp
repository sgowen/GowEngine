//
//  PolygonBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "PolygonBatcher.hpp"

#include "Color.hpp"
#include "Rektangle.hpp"
#include "Triangle.hpp"
#include "Shader.hpp"
#include "OpenGLWrapper.hpp"

PolygonBatcher::PolygonBatcher(int maxBatchSize, bool isFill) :
_maxBatchSize(maxBatchSize),
_isFill(isFill),
_quadVertexBuffer(0),
_quadIndexBuffer(0),
_triangleVertexBuffer(0)
{
    _quadVertices.reserve(_maxBatchSize * NUM_VERTICES_PER_RECTANGLE);
    _triangleVertices.resize(_maxBatchSize * NUM_VERTICES_PER_TRIANGLE);
}

void PolygonBatcher::createDeviceDependentResources()
{
    _quadVertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _quadVertices.capacity());
    _quadIndexBuffer = OGL.loadRektangleIndexBuffer(_maxBatchSize);
    _triangleVertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _triangleVertices.capacity());
}

void PolygonBatcher::releaseDeviceDependentResources()
{
    OGL.unloadBuffer(_quadVertexBuffer);
    OGL.unloadBuffer(_quadIndexBuffer);
    OGL.unloadBuffer(_triangleVertexBuffer);
}

void PolygonBatcher::begin()
{
    _quadVertices.clear();
    _triangleVertices.clear();
}

void PolygonBatcher::addRektangle(Rektangle& r)
{
    float left = r.left();
    float bottom = r.bottom();
    float right = left + r._width;
    float top = bottom + r._height;
    
    addRektangle(left, bottom, right, top);
}

void PolygonBatcher::addRektangle(float left, float bottom, float right, float top)
{
    assert((_quadVertices.size() / NUM_VERTICES_PER_RECTANGLE) < _maxBatchSize);
    
    _quadVertices.emplace_back(left, bottom);
    _quadVertices.emplace_back(left, top);
    _quadVertices.emplace_back(right, top);
    _quadVertices.emplace_back(right, bottom);
}

void PolygonBatcher::addTriangle(Triangle& t)
{
    addTriangle(
                t.getPointA()._x, t.getPointA()._y,
                t.getPointB()._x, t.getPointB()._y,
                t.getPointC()._x, t.getPointC()._y
                );
}

void PolygonBatcher::addTriangle(float leftX, float leftY, float topX, float topY, float rightX, float rightY)
{
    assert((_triangleVertices.size() / NUM_VERTICES_PER_TRIANGLE) < _maxBatchSize);
    
    _triangleVertices.emplace_back(leftX, leftY);
    _triangleVertices.emplace_back(topX, topY);
    _triangleVertices.emplace_back(rightX, rightY);
}

void PolygonBatcher::end(Shader& s, mat4& matrix, const Color& c)
{
    if (!_quadVertices.empty())
    {
        int numQuads = (int)_quadVertices.size() / NUM_VERTICES_PER_RECTANGLE;
        
        OGL.bindVertexBuffer(_quadVertexBuffer, sizeof(VERTEX_2D) * _quadVertices.size(), &_quadVertices[0]);
        OGL.bindShader(s);
        OGL.bindMatrix(s, "u_Matrix", matrix);
        OGL.bindColor(s, "u_Color", c);
        
        if (_isFill)
        {
            OGL.drawIndexed(GL_TRIANGLES, _quadIndexBuffer, numQuads);
        }
        else
        {
            for (uint32_t i = 0; i < numQuads; ++i)
            {
                OGL.drawIndexed(GL_LINE_STRIP, _quadIndexBuffer, 1, i);
            }
        }
        
        OGL.unbindShader(s);
        OGL.unbindVertexBuffer();
    }
    
    if (!_triangleVertices.empty())
    {
        OGL.bindVertexBuffer(_triangleVertexBuffer, sizeof(VERTEX_2D) * _triangleVertices.size(), &_triangleVertices[0]);
        OGL.bindShader(s);
        OGL.bindMatrix(s, "u_Matrix", matrix);
        OGL.bindColor(s, "u_Color", c);
        
        if (_isFill)
        {
            OGL.draw(GL_TRIANGLES, 0, (int)_triangleVertices.size());
        }
        else
        {
            // FIXME, leaves out the last side for some reason, call drawIndexed instead?
            OGL.draw(GL_LINE_STRIP, 0, (int)_triangleVertices.size());
        }
        
        OGL.unbindShader(s);
        OGL.unbindVertexBuffer();
    }
}
