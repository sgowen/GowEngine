//
//  Framebuffer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>

class Framebuffer
{
public:
    uint16_t _width;
    uint16_t _height;
    const std::string _filterMin;
    const std::string _filterMag;
    uint32_t _texture;
    uint32_t _fbo;

    Framebuffer(uint16_t width, uint16_t height, std::string filterMin = "SMOOTH", std::string filterMag = "SMOOTH");
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
};
