//
//  FramebufferRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/8/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

FramebufferRenderer::FramebufferRenderer() :
_vertexBuffer(0),
_indexBuffer(0)
{
    // Empty
}

void FramebufferRenderer::createDeviceDependentResources()
{
    VERTEX_2D vertices[] = {VERTEX_2D(-1, -1), VERTEX_2D(-1, 1), VERTEX_2D(1, 1), VERTEX_2D(1, -1)};
    
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTICES_PER_RECTANGLE, &vertices[0]);
    _indexBuffer = OGL.loadRektangleIndexBuffer(1);
}

void FramebufferRenderer::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void FramebufferRenderer::renderFramebuffer(Shader& s, Framebuffer& fb)
{
    OGL.enableBlending(false);
    
    OGL.bindVertexBuffer(_vertexBuffer);
    OGL.bindShader(s);
    OGL.bindTexture(s, "u_texture", 0, fb._texture);
    
    OGL.drawTrianglesIndexed(_indexBuffer, 1);
    
    OGL.unbindTexture(0);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
    
    OGL.enableBlending(true);
}
