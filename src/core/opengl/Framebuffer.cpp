//
//  Framebuffer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Framebuffer::Framebuffer(uint16_t width, uint16_t height, std::string filterMin, std::string filterMag) :
_width(width),
_height(height),
_filterMin(filterMin),
_filterMag(filterMag),
_texture(0),
_fbo(0)
{
    // Empty
}

void Framebuffer::createDeviceDependentResources()
{
    OGL.loadFramebuffer(*this);
}

void Framebuffer::destroyDeviceDependentResources()
{
    OGL.unloadFramebuffer(*this);
}
