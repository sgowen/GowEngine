//
//  CircleBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define DEGREE_SPACING 6
#define NUM_VERTICES_PER_FULL_CIRCLE 360 / DEGREE_SPACING

CircleBatcher::CircleBatcher(uint32_t maxBatchSize) :
_maxBatchSize(maxBatchSize),
_vertexBuffer(0)
{
    _vertices.reserve(_maxBatchSize * NUM_VERTICES_PER_FULL_CIRCLE);
}

void CircleBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _vertices.capacity());
}

void CircleBatcher::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
}

void CircleBatcher::begin()
{
    _vertices.clear();
    _circles.clear();
}

void CircleBatcher::addCircle(Circle& c)
{
    addCircle(c._center._x, c._center._y, c._radius);
}

void CircleBatcher::addCircle(float x, float y, float radius)
{
    assert((_vertices.size() / NUM_VERTICES_PER_FULL_CIRCLE) < _maxBatchSize);
    
    size_t numExistingPoints = _vertices.size();
    
    for (int i = 0; i <= 360; i += DEGREE_SPACING)
    {
        float rad = DEGREES_TO_RADIANS(i);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        _vertices.emplace_back(cos * radius + x, sin * radius + y);
        _vertices.emplace_back(x, y);
    }
    
    size_t numPoints = _vertices.size() - numExistingPoints;
    _circles.push_back(numPoints);
}

void CircleBatcher::addPartialCircle(Circle& c, int arcDegrees)
{
    addPartialCircle(c._center._x, c._center._y, c._radius, arcDegrees);
}

void CircleBatcher::addPartialCircle(float x, float y, float radius, int arcDegrees)
{
    assert((_vertices.size() / NUM_VERTICES_PER_FULL_CIRCLE) < _maxBatchSize);
    
    size_t numExistingPoints = _vertices.size();
    
    for (int i = 90; i < (450 - arcDegrees); i += DEGREE_SPACING)
    {
        float rad = DEGREES_TO_RADIANS(i);
        float cos = cosf(rad);
        float sin = sinf(rad);
        
        _vertices.emplace_back(cos * radius + x, sin * radius + y);
        _vertices.emplace_back(x, y);
    }
    
    float rad = DEGREES_TO_RADIANS(450 - arcDegrees);
    float cos = cosf(rad);
    float sin = sinf(rad);
    
    _vertices.emplace_back(cos * radius + x, sin * radius + y);
    _vertices.emplace_back(x, y);
    
    size_t numPoints = _vertices.size() - numExistingPoints;
    _circles.push_back(numPoints);
}

void CircleBatcher::end(Shader& s, mat4& matrix, const Color& c)
{
    if (_circles.empty())
    {
        return;
    }
    
    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_Matrix", matrix);
    OGL.bindColor(s, "u_Color", c);

    uint32_t offset = 0;
    for (std::vector<size_t>::iterator i = _circles.begin(); i != _circles.end(); ++i)
    {
        uint32_t numPoints = (uint32_t)(*i);
        OGL.draw(GL_TRIANGLE_STRIP, offset, numPoints);
        offset += numPoints;
    }

    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
