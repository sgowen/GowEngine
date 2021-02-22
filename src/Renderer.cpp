//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Renderer.hpp"

#include "Entity.hpp"
#include "EntityRenderController.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "RapidJSONUtil.hpp"
#include "ResourceManager.hpp"

#include <assert.h>
#include <algorithm>

#include <rapidjson/document.h>

Renderer::Renderer() :
_screenRenderer()
{
    // TODO
}

void Renderer::initWithJSONFile(const char* filePath)
{
    AssetHandler* ah = AssetHandlerFactory::create();
    FileData jsonData = ah->loadAsset(filePath);
    initWithJSON((const char*)jsonData._data);
    ah->releaseAsset(jsonData);
    AssetHandlerFactory::destroy(ah);
}

void Renderer::initWithJSON(const char* json)
{
    // TODO
}

void Renderer::createDeviceDependentResources()
{
    _screenRenderer.createDeviceDependentResources();
}

void Renderer::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight)
{
    _screenRenderer.onWindowSizeChanged(screenWidth, screenHeight);
}

void Renderer::releaseDeviceDependentResources()
{
    _screenRenderer.releaseDeviceDependentResources();
}

void Renderer::updateMatrix(std::string key, float l, float r, float b, float t, float n, float f)
{
    auto q = _matrices.find(key);
    assert(q != _matrices.end());
    mat4& m = q->second;
    
    mat4_identity(m);
    mat4_ortho(m, l, r, b, t, n, f);
}

void Renderer::addEntitiesToSpritesBatcher(std::string key, std::vector<Entity*>& entities)
{
    auto q = _spriteBatchers.find(key);
    assert(q != _spriteBatchers.end());
    SpriteBatcher& sb = q->second;
    
    for (Entity* e : entities)
    {
        e->renderController()->addSprite(sb);
    }
}

void Renderer::renderToScreen(std::string framebufferKey, std::string shader)
{
    auto q = _framebuffers.find(framebufferKey);
    assert(q != _framebuffers.end());
    
    Shader& s = RES_MGR.shader(shader);
    _screenRenderer.renderToScreen(s, q->second);
}
