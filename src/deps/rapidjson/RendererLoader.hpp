//
//  RendererLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/Renderer.hpp"

class RendererLoader
{
public:
    static Renderer initWithJSONFile(std::string filePath);
    static Renderer initWithJSON(const char* json);

private:
    RendererLoader();
    ~RendererLoader();
    RendererLoader(const RendererLoader&);
    RendererLoader& operator=(const RendererLoader&);
};
