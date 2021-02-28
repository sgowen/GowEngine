//
//  Renderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "CircleBatcher.hpp"
#include "FontBatcher.hpp"
#include "Framebuffer.hpp"
#include "Matrix.hpp"
#include "RektangleBatcher.hpp"
#include "SpriteBatcher.hpp"
#include "TextView.hpp"
#include "TriangleBatcher.hpp"
#include "ScreenRenderer.hpp"

#include <map>

typedef void (*RenderFunc)(Renderer& r);

class Entity;

class Renderer
{
    friend class RendererLoader;
    
public:
    Renderer();
    
    void setRenderFunc(RenderFunc rf);
    void render();
    void createDeviceDependentResources();
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight);
    void destroyDeviceDependentResources();
    void bindFramebuffer(std::string framebufferKey = "main", bool enableBlending = true);
    void clearFramebuffer(const Color& c = Color::CLEAR);
    void updateMatrix(float l, float r, float b, float t, float n = -1, float f = 1, std::string matrixKey = "main");
    void rektangleBatcherBegin(std::string rektangleBatcherKey = "main");
    void rektangleBatcherAddRektangle(Rektangle& r, std::string rektangleBatcherKey = "main");
    void rektangleBatcherEnd(const Color& c, std::string matrixKey = "main", std::string shaderKey = "geometry", std::string rektangleBatcherKey = "main");
    void renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, float angle = 0, bool flipX = false, std::string matrixKey = "main", std::string shaderKey = "texture", std::string spriteBatcherKey = "main");
    void spriteBatcherBegin(std::string spriteBatcherKey = "main");
    void spriteBatcherAddEntities(std::vector<Entity*>& entities, std::string spriteBatcherKey = "main");
    void spriteBatcherEnd(std::string textureKey, std::string matrixKey = "main", std::string shaderKey = "texture", std::string spriteBatcherKey = "main", const Color& colorFactor = Color::ONE);
    void setTextVisible(std::string textViewKey, bool isVisible);
    void setText(std::string textViewKey, std::string text);
    void hideAllText();
    void renderText(std::string fontBatcherKey = "main", std::string shaderKey = "texture");
    void renderToScreen(std::string framebufferKey = "main", std::string shaderKey = "framebuffer");
    
    CircleBatcher& circleBatcher(std::string key = "main");
    FontBatcher& fontBatcher(std::string key = "main");
    Framebuffer& framebuffer(std::string key = "main");
    Matrix& matrix(std::string key = "main");
    RektangleBatcher& rektangleBatcher(std::string key = "main");
    SpriteBatcher& spriteBatcher(std::string key = "main");
    TextView& textView(std::string key = "main");
    TriangleBatcher& triangleBatcher(std::string key = "main");
    
private:
    RenderFunc _renderFunc;
    std::map<std::string, CircleBatcher> _circleBatchers;
    std::map<std::string, FontBatcher> _fontBatchers;
    std::map<std::string, Framebuffer> _framebuffers;
    std::map<std::string, Matrix> _matrices;
    std::map<std::string, RektangleBatcher> _rektangleBatchers;
    std::map<std::string, SpriteBatcher> _spriteBatchers;
    std::map<std::string, TextView> _textViews;
    std::map<std::string, TriangleBatcher> _triangleBatchers;
    ScreenRenderer _screenRenderer;
};
