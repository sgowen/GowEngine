//
//  ScreenRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct Shader;
struct Framebuffer;

class ScreenRenderer
{
public:
    ScreenRenderer();
    ~ScreenRenderer();
    
    void createDeviceDependentResources();
    void onWindowSizeChanged(int screenWidth, int screenHeight);
    void releaseDeviceDependentResources();
    void renderToScreen(Shader& s, Framebuffer& fb);
    void renderFramebuffer(Shader& s, Framebuffer& fb);
    
private:
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
    int _screenWidth;
    int _screenHeight;
};
