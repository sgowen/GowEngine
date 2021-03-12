//
//  ScreenRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ScreenRenderer::ScreenRenderer() :
_vertexBuffer(0),
_indexBuffer(0),
_screenWidth(0),
_screenHeight(0)
{
    // Empty
}

void ScreenRenderer::createDeviceDependentResources()
{
    VERTEX_2D vertices[] = {VERTEX_2D(-1, -1), VERTEX_2D(-1, 1), VERTEX_2D(1, 1), VERTEX_2D(1, -1)};
    
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTICES_PER_RECTANGLE, &vertices[0]);
    _indexBuffer = OGL.loadRektangleIndexBuffer(1);
}

void ScreenRenderer::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight)
{
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
}

void ScreenRenderer::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void ScreenRenderer::renderToScreen(Shader& s, Framebuffer& fb)
{
    OGL.bindScreenFramebuffer(_screenWidth, _screenHeight, Color::BLACK);
    OGL.enableBlending(false);
    
    renderFramebuffer(s, fb);
}

void ScreenRenderer::renderFramebuffer(Shader& s, Framebuffer& fb)
{
    OGL.bindVertexBuffer(_vertexBuffer);
    OGL.bindShader(s);
    OGL.bindTexture(s, "u_Texture", 0, fb._texture);
    
    OGL.drawIndexed(GL_TRIANGLES, _indexBuffer, 1);
    
    OGL.unbindTexture(0);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
