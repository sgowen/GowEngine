//
//  ShockwaveRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/7/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ShockwaveRenderer::ShockwaveRenderer() :
_vertexBuffer(0),
_indexBuffer(0)
{
    // Empty
}

void ShockwaveRenderer::createDeviceDependentResources()
{    
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTICES_PER_RECTANGLE);
    _indexBuffer = OGL.loadRektangleIndexBuffer(1);
}

void ShockwaveRenderer::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void ShockwaveRenderer::render(Shader& s, Framebuffer& fb, Matrix& m, float x, float y, float timeElapsed, bool isTransforming)
{
    float x1 = m._desc._left;
    float y1 = m._desc._bottom;
    float x2 = m._desc._right;
    float y2 = m._desc._top;
    
    _vertices.clear();
    _vertices.emplace_back(x1, y1);
    _vertices.emplace_back(x1, y2);
    _vertices.emplace_back(x2, y2);
    _vertices.emplace_back(x2, y1);
    
    OGL.enableBlending(false);
    
    OGL.bindVertexBuffer(_vertexBuffer, sizeof(VERTEX_2D) * _vertices.size(), &_vertices[0]);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_matrix", m._matrix);
    OGL.bindTexture(s, "u_texture", 0, fb._texture);
    OGL.bindFloat(s, "u_centerX", x);
    OGL.bindFloat(s, "u_centerY", y);
    OGL.bindFloat(s, "u_timeElapsed", timeElapsed * 1.2f + 0.1f);
    OGL.bindInt(s, "u_isTransforming", isTransforming ? 1 : 0);
    
    OGL.drawTrianglesIndexed(_indexBuffer, 1);
    
    OGL.unbindTexture(0);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
    
    OGL.enableBlending(true);
}
