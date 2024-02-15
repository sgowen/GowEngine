//
//  RendererLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class Renderer;

class RendererLoader
{
public:
    static void initWithJSONFile(Renderer& renderer, std::string filePath);
    static void initWithJSON(Renderer& renderer, const char* json);

private:
    RendererLoader();
    ~RendererLoader();
    RendererLoader(const RendererLoader&);
    RendererLoader& operator=(const RendererLoader&);
};
