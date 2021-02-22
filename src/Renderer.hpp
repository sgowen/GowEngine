//
//  Renderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"
#include "FontBatcher.hpp"
#include "Framebuffer.hpp"
#include "RektangleBatcher.hpp"
#include "TriangleBatcher.hpp"
#include "CircleBatcher.hpp"
#include "ScreenRenderer.hpp"
#include "SpriteBatcher.hpp"
#include "TextView.hpp"

#include <map>

class Entity;

class Renderer
{
public:
    Renderer();
    
    void initWithJSONFile(const char* filePath);
    void initWithJSON(const char* json);
    void createDeviceDependentResources();
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight);
    void releaseDeviceDependentResources();
    void updateMatrix(std::string key, float l, float r, float b, float t, float n = -1, float f = 1);
    void addEntitiesToSpritesBatcher(std::string key, std::vector<Entity*>& entities);
    void renderToScreen(std::string framebufferKey, std::string shader = "framebuffer");
    
private:
    ScreenRenderer _screenRenderer;
    std::map<std::string, CircleBatcher> _circleBatchers;
    std::map<std::string, FontBatcher> _fontBatchers;
    std::map<std::string, Framebuffer> _framebuffers;
    std::map<std::string, mat4> _matrices;
    std::map<std::string, RektangleBatcher> _rektangleBatchers;
    std::map<std::string, SpriteBatcher> _spriteBatchers;
    std::map<std::string, TextView> _textViews;
    std::map<std::string, TriangleBatcher> _triangleBatchers;
};
