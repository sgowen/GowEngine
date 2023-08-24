//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Renderer::Renderer() :
_nosPhysicsRenderer(2048),
_screenRenderer(),
_pixelToUnitRatio(1)
{
    // Empty
}

void Renderer::createDeviceDependentResources()
{
    _nosPhysicsRenderer.createDeviceDependentResources();
    _screenRenderer.createDeviceDependentResources();
    
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
        pair.second.createDeviceDependentResources();
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
}

void Renderer::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight)
{
    _screenRenderer.onWindowSizeChanged(screenWidth, screenHeight);
}

void Renderer::destroyDeviceDependentResources()
{
    _nosPhysicsRenderer.destroyDeviceDependentResources();
    _screenRenderer.destroyDeviceDependentResources();
    
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
        pair.second.destroyDeviceDependentResources();
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
}

void Renderer::renderLoadingView(uint32_t stateTime)
{
    clearFramebuffer();
    
    if (!ASSETS_MGR.isShaderLoaded("texture") ||
        !ASSETS_MGR.isFontLoaded("main"))
    {
        return;
    }
    
    std::string loadingText = "Loading";
    int numDots = stateTime % 32 / 8;
    for (int i = 0; i < numDots; ++i)
    {
        loadingText += ".";
    }
    
    FontBatcher& fb = fontBatcher("main");
    Shader& s = ASSETS_MGR.shader("texture");
    fb.begin();
    fb.addText(*this, loadingText, TEXA_LEFT, 0.80f, 0.04f, 0.02f);
    fb.end(*this, s);
}

void Renderer::bindFramebuffer(std::string framebufferKey, bool enableBlending)
{
    Framebuffer& fb = framebuffer(framebufferKey);
    OGL.bindFramebuffer(fb);
    OGL.enableBlending(enableBlending);
}

void Renderer::clearFramebuffer(Color& c)
{
    OGL.clearFramebuffer(c);
}

void Renderer::renderImageViews(float angle, bool flipX, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    Matrix& m = matrix(matrixKey);
    float l = m._desc._left;
    float b = m._desc._bottom;
    float w = m._desc.width();
    float h = m._desc.height();
    for (auto& pair : _imageViews)
    {
        ImageView& iv = pair.second;
        renderSprite(iv._texture, iv._textureRegion, l + w * iv._xWeight, b + h * iv._yWeight, w * iv._widthWeight, h * iv._heightWeight, 0, angle, flipX, matrixKey, shaderKey, spriteBatcherKey);
    }
}

void Renderer::updateMatrix(float l, float r, float b, float t, float n, float f, std::string matrixKey)
{
    Matrix& m = matrix(matrixKey);
    m.ortho(l, r, b, t, n, f);
}

void Renderer::updateMatrix(MatrixDescriptor& desc, std::string matrixKey)
{
    updateMatrix(desc._left, desc._right, desc._bottom, desc._top, desc._near, desc._far, matrixKey);
}

void Renderer::updateMatrixCenteredOnEntity(Entity* e, float maxRight, float maxTop, float scale, std::string matrixKey)
{
    if (e == nullptr)
    {
        return;
    }
    
    Matrix& m = matrix(matrixKey);
    MatrixDescriptor md = m._base;
    float width = md.width() * scale;
    float height = md.height() * scale;
    md._right = CLAMP(e->position()._x + width / 2, 0, maxRight);
    md._top = CLAMP(e->position()._y + height / 2, 0, maxTop);
    
    md._left = md._right - width;
    if (md._left < 0)
    {
        float diff = -md._left;
        md._left = 0;
        md._right += diff;
    }
    
    md._bottom = md._top - height;
    if (md._bottom < 0)
    {
        float diff = -md._bottom;
        md._bottom = 0;
        md._top += diff;
    }
    
    updateMatrix(md, matrixKey);
}

void Renderer::rektangleBatcherBegin(std::string rektangleBatcherKey)
{
    rektangleBatcher(rektangleBatcherKey).begin();
}

void Renderer::rektangleBatcherAddRektangle(Rektangle& r, std::string rektangleBatcherKey)
{
    rektangleBatcher(rektangleBatcherKey).addRektangle(r);
}

void Renderer::rektangleBatcherEnd(Color& c, std::string matrixKey, std::string shaderKey, std::string rektangleBatcherKey)
{
    RektangleBatcher& rb = rektangleBatcher(rektangleBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS_MGR.shader(shaderKey);
    
    rb.end(s, m._matrix, c);
}

void Renderer::renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, uint16_t stateTime, float angle, bool flipX, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS_MGR.shader(shaderKey);
    Texture& t = ASSETS_MGR.texture(textureKey);
    TextureRegion& tr = ASSETS_MGR.textureRegion(textureRegionKey, stateTime);
    
    sb.begin();
    sb.addSprite(tr, x, y, width, height, angle, flipX);
    sb.end(s, m._matrix, t);
}

void Renderer::renderParallaxLayers(std::vector<Entity*>& layers, std::string texture)
{
    Matrix& m = matrix();
    
    spriteBatcherBegin();
    for (Entity* e : layers)
    {
        std::string textureRegionKey = e->renderController()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        
        if (textureForRegionKey == texture)
        {
            TextureRegion& trRaw = ASSETS_MGR.textureRegion(e->renderController()->getTextureMapping(),  e->stateTime());
            
            Texture& t = ASSETS_MGR.texture(texture);
            float parallaxSpeedRatio = e->data().getFloat("parallaxSpeedRatio");
            uint8_t gameToPixelRatio = t._width / e->width();
            uint16_t x = (m._desc._left * parallaxSpeedRatio) * gameToPixelRatio;
            x %= t._width;
            TextureRegion tr = trRaw.copyWithX(x);
            
            spriteBatcher().addSprite(tr, e->position()._x + m._desc._left, e->position()._y, e->width(), e->height(), e->angle(), e->isXFlipped());
            
            float matrixWidthRemaining = m._desc.width() - e->width();
            for (int i = 1; matrixWidthRemaining > 0; matrixWidthRemaining -= e->width(), ++i)
            {
                x += t._width;
                x %= t._width;
                TextureRegion trX = tr.copyWithX(x);
                
                spriteBatcher().addSprite(trX, e->position()._x + m._desc._left + e->width() * i, e->position()._y, e->width(), e->height(), e->angle(), e->isXFlipped());
            }
        }
    }
    spriteBatcherEnd(texture);
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
        assert(e != nullptr);
        
        spriteBatcherAddEntity(sb, *e);
    }
}

void Renderer::spriteBatcherAddEntity(Entity* e, std::string spriteBatcherKey)
{
    assert(e != nullptr);
    
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    spriteBatcherAddEntity(sb, *e);
}

void Renderer::spriteBatcherEnd(std::string textureKey, std::string matrixKey, std::string shaderKey, std::string spriteBatcherKey, Color& colorFactor)
{
    SpriteBatcher& sb = spriteBatcher(spriteBatcherKey);
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS_MGR.shader(shaderKey);
    Texture& t = ASSETS_MGR.texture(textureKey);
    
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

void Renderer::renderTextViews(std::string fontBatcherKey, std::string shaderKey, Color& colorFactor)
{
    FontBatcher& fb = fontBatcher(fontBatcherKey);
    Shader& s = ASSETS_MGR.shader(shaderKey);
    fb.begin();
    for (auto& pair : _textViews)
    {
        fb.addText(*this, pair.second);
    }
    fb.end(*this, s, colorFactor);
}

void Renderer::renderNosPhysics(NosPhysicsWorld* world, std::string matrixKey, std::string shaderKey)
{
    Matrix& m = matrix(matrixKey);
    Shader& s = ASSETS_MGR.shader(shaderKey);
    _nosPhysicsRenderer.render(world, &m._matrix, &s);
}

void Renderer::renderToScreen(std::string framebufferKey, std::string shaderKey)
{
    Shader& s = ASSETS_MGR.shader(shaderKey);
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

void Renderer::spriteBatcherAddEntity(SpriteBatcher& sb, Entity& e)
{
    TextureRegion& tr = ASSETS_MGR.textureRegion(e.renderController()->getTextureMapping(),  e.stateTime());
    assert(tr._width % _pixelToUnitRatio == 0);
    assert(tr._height % _pixelToUnitRatio == 0);
    float realWidth = tr._width / _pixelToUnitRatio;
    float realHeight = tr._height / _pixelToUnitRatio;
    sb.addSprite(tr, e.position()._x, e.position()._y, realWidth, realHeight, e.angle(), e.isXFlipped());
}
