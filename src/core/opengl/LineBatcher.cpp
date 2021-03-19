//
//  LineBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/15/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

LineBatcher::LineBatcher(uint32_t maxBatchSize) :
_maxBatchSize(maxBatchSize),
_vertexBuffer(0)
{
    _vertices.reserve(_maxBatchSize * NUM_VERTICES_PER_LINE);
}

void LineBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * _vertices.capacity());
}

void LineBatcher::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
}

void LineBatcher::begin()
{
    _vertices.clear();
}

void LineBatcher::addLine(Line& line)
{
    float oX = line._origin._x;
    float oY = line._origin._y;
    float eX = line._end._x;
    float eY = line._end._y;
    
    addLine(oX, oY, eX, eY);
}

void LineBatcher::addLine(float oX, float oY, float eX, float eY)
{
    assert((_vertices.size() / NUM_VERTICES_PER_LINE) < _maxBatchSize);
    
    _vertices.emplace_back(oX, oY);
    _vertices.emplace_back(eX, eY);
}

void LineBatcher::end(Shader& s, mat4& matrix, const Color& c)
{
    if (_vertices.empty())
    {
        return;
    }
    
    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_Matrix", matrix);
    OGL.bindColor(s, "u_Color", c);
    
    OGL.draw(OpenGLWrapper::MODE_LINES, 0, (int)_vertices.size());
    
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
