//
//  LightRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/14/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "LightRenderer.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "OpenGLWrapper.hpp"

#include <assert.h>

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

void LightRenderer::releaseDeviceDependentResources()
{
    OGL.unloadBuffer(_vertexBuffer);
    OGL.unloadBuffer(_indexBuffer);
}

void LightRenderer::resetLights()
{
    for (int i = 0; i < NUM_LIGHTS; ++i)
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

void LightRenderer::render(Shader& s, mat4& matrix, uint32_t texture, uint32_t normalMap)
{
    OGL.bindVertexBuffer(_vertexBuffer);
    OGL.bindShader(s);
    OGL.bindMatrix(s, "u_Matrix", matrix);
    OGL.bindFloat4Array(s, "u_LightPositions", _numLights[0], _lightPositions);
    OGL.bindFloat4Array(s, "u_LightColors", _numLights[0], _lightColors);
    OGL.bindFloat4(s, "u_AmbientColor", _ambientColor);
    OGL.bindFloat4(s, "u_Falloff", _fallOff);
    OGL.bindInt4(s, "u_NumLights", _numLights);
    OGL.bindTexture(s, "u_Texture", 0, texture);
    OGL.bindTexture(s, "u_NormalMap", 1, normalMap);

    OGL.drawIndexed(GL_TRIANGLES, _indexBuffer, 1);

    OGL.unbindTexture(0);
    OGL.unbindTexture(1);
    OGL.unbindShader(s);
    OGL.unbindVertexBuffer();
}
