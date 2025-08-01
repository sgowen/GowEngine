//
//  SpriteBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

SpriteBatcher::SpriteBatcher(uint32_t maxBatchSize) :
_maxBatchSize(maxBatchSize),
_vertexBuffer(0),
_indexBuffer(0)
{
    // Empty
}

void SpriteBatcher::createDeviceDependentResources()
{
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D_TEXTURE) * _maxBatchSize * NUM_VERTICES_PER_RECTANGLE);
    _indexBuffer = OGL.loadRektangleIndexBuffer(_maxBatchSize);
}

void SpriteBatcher::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void SpriteBatcher::begin()
{
    _vertices.clear();
}

void SpriteBatcher::addSprite(TextureRegion& tr, float x, float y, float width, float height, float angle, bool flipX)
{
    assert((_vertices.size() / NUM_VERTICES_PER_RECTANGLE) < _maxBatchSize);
    
    float halfWidth = width / 2;
    float halfHeight = height / 2;
    
    vec4 xFinal, yFinal, uFinal;
    
    if (angle == 0)
    {
        float left = x - halfWidth;
        float bottom = y - halfHeight;
        float right = x + halfWidth;
        float top = y + halfHeight;
        
        xFinal[0] = left;
        yFinal[0] = bottom;
        xFinal[1] = left;
        yFinal[1] = top;
        xFinal[2] = right;
        yFinal[2] = top;
        xFinal[3] = right;
        yFinal[3] = bottom;
    }
    else
    {
        float cos = cosf(angle);
        float sin = sinf(angle);
        
        float x1 = -halfWidth * cos - (-halfHeight) * sin;
        float y1 = -halfWidth * sin + (-halfHeight) * cos;
        
        float x2 = halfWidth * cos - (-halfHeight) * sin;
        float y2 = halfWidth * sin + (-halfHeight) * cos;
        
        float x3 = halfWidth * cos - halfHeight * sin;
        float y3 = halfWidth * sin + halfHeight * cos;
        
        float x4 = -halfWidth * cos - halfHeight * sin;
        float y4 = -halfWidth * sin + halfHeight * cos;
        
        x1 += x;
        y1 += y;
        
        x2 += x;
        y2 += y;
        
        x3 += x;
        y3 += y;
        
        x4 += x;
        y4 += y;
        
        xFinal[0] = x1;
        yFinal[0] = y1;
        xFinal[1] = x4;
        yFinal[1] = y4;
        xFinal[2] = x3;
        yFinal[2] = y3;
        xFinal[3] = x2;
        yFinal[3] = y2;
    }
    
    if (flipX)
    {
        uFinal[0] = tr._u2;
        uFinal[1] = tr._u2;
        uFinal[2] = tr._u1;
        uFinal[3] = tr._u1;
    }
    else
    {
        uFinal[0] = tr._u1;
        uFinal[1] = tr._u1;
        uFinal[2] = tr._u2;
        uFinal[3] = tr._u2;
    }
    
    _vertices.emplace_back(xFinal[0], yFinal[0], uFinal[0], tr._v2);
    _vertices.emplace_back(xFinal[1], yFinal[1], uFinal[1], tr._v1);
    _vertices.emplace_back(xFinal[2], yFinal[2], uFinal[2], tr._v1);
    _vertices.emplace_back(xFinal[3], yFinal[3], uFinal[3], tr._v2);
}

void SpriteBatcher::end(Shader& s, mat4& matrix, Texture& t, Color& colorFactor)
{
    if (_vertices.empty())
    {
        return;
    }
    
    uint32_t numSprites = (uint32_t)_vertices.size() / NUM_VERTICES_PER_RECTANGLE;
    
    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D_TEXTURE) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_matrix", matrix);
    OGL.bindColor(s, "u_color", colorFactor);
    OGL.bindTexture(s, "u_texture", 0, t);
    
    OGL.drawTrianglesIndexed(_indexBuffer, numSprites);
    
    OGL.unbindTexture(0);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
