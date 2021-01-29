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

#define NUM_LIGHTS 12

class LightRenderer
{
public:
    LightRenderer();
    ~LightRenderer() {}
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void resetLights();
    void configAmbientLight(float r, float g, float b, float a);
    void configureFallOff(float x, float y, float z);
    void addLight(float lightPosX, float lightPosY, float lightPosZ, float lightColorR, float lightColorG, float lightColorB, float lightColorA);
    void render(Shader& s, mat4& matrix, uint32_t texture, uint32_t normalMap);
    
private:
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    vec4 _lightPositions[NUM_LIGHTS];
    vec4 _lightColors[NUM_LIGHTS]; // Light RGB and intensity (alpha)
    vec4 _ambientColor; // Ambient RGB and intensity (alpha)
    vec4 _fallOff; // Attenuation coefficients for light falloff
    ivec4 _numLights;
    int _lightIndex;
};
