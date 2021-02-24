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
#include "OpenGLWrapper.hpp"

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

void Renderer::bindFramebuffer(std::string framebufferKey, bool enableBlending)
{
    Framebuffer& fb = framebuffer(framebufferKey);
    OGL.bindFramebuffer(fb);
    OGL.enableBlending(enableBlending);
}

void Renderer::clearFramebuffer(const Color& c)
{
    OGL.clearFramebuffer(c);
}

void Renderer::updateMatrix(float l, float r, float b, float t, std::string matrixKey, float n, float f)
{
    Matrix& m = matrix(matrixKey);
    m.identity();
    m.ortho(l, r, b, t, n, f);
}

void Renderer::renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, float angle, bool flipX, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = RES_MGR.shader(shaderKey);
    Texture& t = RES_MGR.texture(textureKey);
    TextureRegion& tr = RES_MGR.findTextureRegion(textureRegionKey);
    
    sb.begin();
    sb.addSprite(tr, x, y, width, height, angle, flipX);
    sb.end(s, m._matrix, t);
}

void Renderer::spriteBatcherBeginBatch(std::string spriteBatcherKey)
{
    spriteBatcher(spriteBatcherKey).begin();
}

void Renderer::spriteBatcherAddEntities(std::vector<Entity*>& entities, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    for (Entity* e : entities)
    {
        e->renderController()->addSprite(sb);
    }
}

void Renderer::spriteBatcherEndBatch(std::string textureKey, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = RES_MGR.shader(shaderKey);
    Texture& t = RES_MGR.texture(textureKey);
    
    sb.end(s, m._matrix, t);
}

void Renderer::renderText(std::string fontBatcherKey, std::string shaderKey)
{
    FontBatcher& fb = fontBatcher(fontBatcherKey);
    Shader& s = RES_MGR.shader(shaderKey);
    fb.begin();
    for (auto& pair : _textViews)
    {
        fb.addText(pair.second);
    }
    fb.end(s);
}

void Renderer::renderToScreen(std::string framebufferKey, std::string shaderKey)
{
    Shader& s = RES_MGR.shader(shaderKey);
    _screenRenderer.renderToScreen(s, framebuffer(framebufferKey));
}

CircleBatcher& Renderer::circleBatcher(std::string key)
{
    auto q = _circleBatchers.find(key);
    assert(q != _circleBatchers.end());
    return q->second;
}

FontBatcher& Renderer::fontBatcher(std::string key)
{
    auto q = _fontBatchers.find(key);
    assert(q != _fontBatchers.end());
    return q->second;
}

Framebuffer& Renderer::framebuffer(std::string key)
{
    auto q = _framebuffers.find(key);
    assert(q != _framebuffers.end());
    return q->second;
}

Matrix& Renderer::matrix(std::string key)
{
    auto q = _matrices.find(key);
    assert(q != _matrices.end());
    return q->second;
}

RektangleBatcher& Renderer::rektangleBatcher(std::string key)
{
    auto q = _rektangleBatchers.find(key);
    assert(q != _rektangleBatchers.end());
    return q->second;
}

SpriteBatcher& Renderer::spriteBatcher(std::string key)
{
    auto q = _spriteBatchers.find(key);
    assert(q != _spriteBatchers.end());
    return q->second;
}

TextView& Renderer::textView(std::string key)
{
    auto q = _textViews.find(key);
    assert(q != _textViews.end());
    return q->second;
}

TriangleBatcher& Renderer::triangleBatcher(std::string key)
{
    auto q = _triangleBatchers.find(key);
    assert(q != _triangleBatchers.end());
    return q->second;
}
