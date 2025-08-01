//
//  LightRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/14/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

LightRenderer::LightRenderer() :
_lightIndex(0)
{
    resetLights();
}

void LightRenderer::createDeviceDependentResources()
{
    VERTEX_2D vertices[] = {VERTEX_2D(-1, -1), VERTEX_2D(-1, 1), VERTEX_2D(1, 1), VERTEX_2D(1, -1)};
    
    _vertexBuffer = OGL.loadVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTICES_PER_RECTANGLE, &vertices[0]);
    _indexBuffer = OGL.loadRektangleIndexBuffer(1);
}

void LightRenderer::destroyDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void LightRenderer::resetLights()
{
    for (int i = 0; i < NUM_SUPPORTED_LIGHTS; ++i)
    {
        _lightPositions[i][0] = 0;
        _lightPositions[i][1] = 0;
        _lightPositions[i][2] = 0;
        _lightPositions[i][3] = 0;
        
        _lightColors[i][0] = 0;
        _lightColors[i][1] = 0;
        _lightColors[i][2] = 0;
        _lightColors[i][3] = 0;
    }
    
    _ambientColor[0] = 0;
    _ambientColor[1] = 0;
    _ambientColor[2] = 0;
    _ambientColor[3] = 0;
    
    _fallOff[0] = 0;
    _fallOff[1] = 0;
    _fallOff[2] = 0;
    _fallOff[3] = 0;
    
    _numLights[0] = 0;
    _numLights[1] = 0;
    _numLights[2] = 0;
    _numLights[3] = 0;
    
    _lightIndex = 0;
}

void LightRenderer::configAmbientLight(float r, float g, float b, float a)
{
    _ambientColor[0] = r;
    _ambientColor[1] = g;
    _ambientColor[2] = b;
    _ambientColor[3] = a;
}

void LightRenderer::configureFallOff(float x, float y, float z)
{
    _fallOff[0] = x;
    _fallOff[1] = y;
    _fallOff[2] = z;
}

void LightRenderer::addLight(float lightPosX, float lightPosY, float lightPosZ, float lightColorR, float lightColorG, float lightColorB, float lightColorA)
{
    _lightPositions[_lightIndex][0] = lightPosX;
    _lightPositions[_lightIndex][1] = lightPosY;
    _lightPositions[_lightIndex][2] = lightPosZ;
    
    _lightColors[_lightIndex][0] = lightColorR;
    _lightColors[_lightIndex][1] = lightColorG;
    _lightColors[_lightIndex][2] = lightColorB;
    _lightColors[_lightIndex][3] = lightColorA;
    
    _numLights[0] = ++_lightIndex;
}

void LightRenderer::render(Shader& s, Matrix& m, Framebuffer& texture, Framebuffer& normalMap)
{
    OGL.bindVertexBuffer(_vertexBuffer);
    OGL.bindShader(s);
    OGL.bindTexture(s, "u_texture", 0, texture._texture);
    OGL.bindTexture(s, "u_normalMap", 1, normalMap._texture);
    OGL.bindMatrix(s, "u_matrix", m._matrix);
    
    OGL.bindFloat4Array(s, "u_lightPositions", _numLights[0], _lightPositions);
    OGL.bindFloat4Array(s, "u_lightColors", _numLights[0], _lightColors);
    OGL.bindFloat4(s, "u_ambientColor", _ambientColor);
    OGL.bindFloat4(s, "u_falloff", _fallOff);
    OGL.bindInt4(s, "u_numLights", _numLights);

    OGL.drawTrianglesIndexed(_indexBuffer, 1);

    OGL.unbindTexture(0);
    OGL.unbindTexture(1);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
