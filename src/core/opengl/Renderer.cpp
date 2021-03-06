//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Renderer::Renderer() :
_renderFunc(NULL),
_screenRenderer()
{
    // Empty
}

void Renderer::setRenderFunc(RenderFunc rf)
{
    _renderFunc = rf;
}

void Renderer::render()
{
    assert(_renderFunc != NULL);
    
    _renderFunc(*this);
}

void Renderer::createDeviceDependentResources()
{
    for (auto& pair : _circleBatchers)
    {
        pair.second.createDeviceDependentResources();
    }
    
    for (auto& pair : _fontBatchers)
    {
        pair.second.createDeviceDependentResources();
    }
    
    for (auto& pair : _framebuffers)
    {
        OGL.loadFramebuffer(pair.second);
    }
    
    for (auto& pair : _rektangleBatchers)
    {
        pair.second.createDeviceDependentResources();
    }
    
    for (auto& pair : _spriteBatchers)
    {
        pair.second.createDeviceDependentResources();
    }
    
    for (auto& pair : _triangleBatchers)
    {
        pair.second.createDeviceDependentResources();
    }
    
    _screenRenderer.createDeviceDependentResources();
}

void Renderer::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight)
{
    _screenRenderer.onWindowSizeChanged(screenWidth, screenHeight);
}

void Renderer::destroyDeviceDependentResources()
{
    for (auto& pair : _circleBatchers)
    {
        pair.second.destroyDeviceDependentResources();
    }
    
    for (auto& pair : _fontBatchers)
    {
        pair.second.destroyDeviceDependentResources();
    }
    
    for (auto& pair : _framebuffers)
    {
        OGL.unloadFramebuffer(pair.second);
    }
    
    for (auto& pair : _rektangleBatchers)
    {
        pair.second.destroyDeviceDependentResources();
    }
    
    for (auto& pair : _spriteBatchers)
    {
        pair.second.destroyDeviceDependentResources();
    }
    
    for (auto& pair : _triangleBatchers)
    {
        pair.second.destroyDeviceDependentResources();
    }
    
    _screenRenderer.destroyDeviceDependentResources();
}

void Renderer::renderLoadingScreen()
{
    clearFramebuffer();
    
    if (!ASSETS.isShaderLoaded("texture") ||
        !ASSETS.isTextureLoaded("texture_font"))
    {
        return;
    }
    
    FontBatcher& fb = fontBatcher("main");
    Shader& s = ASSETS.shader("texture");
    fb.begin();
    fb.addText(*this, "Loading...", 2, 0.98f, 0.02f, 0.012f);
    fb.end(*this, s);
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

void Renderer::renderImageViews(float angle, bool flipX, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    Matrix& m = matrix();
    float w = m._desc.width();
    float h = m._desc.height();
    for (auto& pair : _imageViews)
    {
        ImageView& iv = pair.second;
        renderSprite(iv._textureName, iv._textureRegionName, w * iv._xWeight, h * iv._yWeight, w * iv._widthWeight, h * iv._heightWeight, angle, flipX, matrixKey, shaderKey, spriteBatcherKey);
    }
}

void Renderer::updateMatrix(float l, float r, float b, float t, float n, float f, std::string matrixKey)
{
    Matrix& m = matrix(matrixKey);
    m.identity();
    m.ortho(l, r, b, t, n, f);
}

void Renderer::rektangleBatcherBegin(std::string rektangleBatcherKey)
{
    rektangleBatcher(rektangleBatcherKey).begin();
}

void Renderer::rektangleBatcherAddRektangle(Rektangle& r, std::string rektangleBatcherKey)
{
    rektangleBatcher(rektangleBatcherKey).addRektangle(r);
}

void Renderer::rektangleBatcherEnd(const Color& c, std::string matrixKey, std::string shaderKey, std::string rektangleBatcherKey)
{
    RektangleBatcher& rb = rektangleBatcher(rektangleBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS.shader(shaderKey);
    
    rb.end(s, m._matrix, c);
}

void Renderer::renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, float angle, bool flipX, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS.shader(shaderKey);
    Texture& t = ASSETS.texture(textureKey);
    TextureRegion& tr = ASSETS.textureRegion(textureRegionKey);
    
    sb.begin();
    sb.addSprite(tr, x, y, width, height, angle, flipX);
    sb.end(s, m._matrix, t);
}

void Renderer::spriteBatcherBegin(std::string spriteBatcherKey)
{
    spriteBatcher(spriteBatcherKey).begin();
}

void Renderer::spriteBatcherAddEntities(std::vector<Entity*>& entities, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    for (Entity* e : entities)
    {
        addSpriteForEntity(sb, *e);
    }
}

void Renderer::addSpriteForEntity(Entity* e, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    addSpriteForEntity(sb, *e);
}

void Renderer::spriteBatcherEnd(std::string textureKey, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey, const Color& colorFactor)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS.shader(shaderKey);
    Texture& t = ASSETS.texture(textureKey);
    
    sb.end(s, m._matrix, t, colorFactor);
}

void Renderer::setTextVisible(std::string textViewKey, bool isVisible)
{
    textView(textViewKey)._visibility = isVisible ? TEXV_VISIBLE : TEXV_HIDDEN;
}

void Renderer::setText(std::string textViewKey, std::string text)
{
    textView(textViewKey)._text = text;
}

void Renderer::hideAllText()
{
    for (auto& pair : _textViews)
    {
        pair.second._visibility = TEXV_HIDDEN;
    }
}

void Renderer::renderTextViews(std::string fontBatcherKey, std::string shaderKey)
{
    FontBatcher& fb = fontBatcher(fontBatcherKey);
    Shader& s = ASSETS.shader(shaderKey);
    fb.begin();
    for (auto& pair : _textViews)
    {
        fb.addText(*this, pair.second);
    }
    fb.end(*this, s);
}

void Renderer::renderToScreen(std::string framebufferKey, std::string shaderKey)
{
    Shader& s = ASSETS.shader(shaderKey);
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

ImageView& Renderer::imageView(std::string key)
{
    auto q = _imageViews.find(key);
    assert(q != _imageViews.end());
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

void Renderer::addSpriteForEntity(SpriteBatcher& sb, Entity& e)
{
    TextureRegion tr = ASSETS.textureRegion(e.renderController()->getTextureMapping(), e.stateTime());
    sb.addSprite(tr, e.position()._x, e.position()._y, e.width(), e.height(), e.angle(), e.isXFlipped());
}
