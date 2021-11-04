//
//  LightRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/14/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"

#include <stdint.h>

struct Shader;
struct Texture;
class Framebuffer;

class LightRenderer
{
public:
    LightRenderer();
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void resetLights();
    void configAmbientLight(float r, float g, float b, float a);
    void configureFallOff(float x, float y, float z);
    void addLight(float lightPosX, float lightPosY, float lightPosZ, float lightColorR, float lightColorG, float lightColorB, float lightColorA);
    void render(Shader& s, mat4& matrix, Framebuffer& texture, Framebuffer& normalMap);
    
private:
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    vec4 _lightPositions[NUM_SUPPORTED_LIGHTS];
    vec4 _lightColors[NUM_SUPPORTED_LIGHTS];
    vec4 _ambientColor;
    vec4 _fallOff;
    ivec4 _numLights;
    int _lightIndex;
};
