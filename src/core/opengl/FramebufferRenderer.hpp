//
//  FramebufferRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 9/8/23.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

struct Shader;
class Framebuffer;

class FramebufferRenderer
{
public:
    FramebufferRenderer();
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    
    void renderFramebuffer(Shader& s, Framebuffer& fb);
private:
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
};
