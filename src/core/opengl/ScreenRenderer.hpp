//
//  ScreenRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct Shader;
class Framebuffer;

class ScreenRenderer
{
public:
    ScreenRenderer();
    
    void createDeviceDependentResources();
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight);
    void destroyDeviceDependentResources();
    void renderToScreen(Shader& s, Framebuffer& fb);
    
private:
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    int _screenWidth;
    int _screenHeight;
    
    void renderFramebuffer(Shader& s, Framebuffer& fb);
};
