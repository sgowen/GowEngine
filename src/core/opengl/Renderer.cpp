//
//  Renderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Renderer::Renderer() :
_box2DPhysicsRenderer(2048),
_nosPhysicsRenderer(2048),
_framebufferRenderer(),
_lightRenderer(),
_screenRenderer(),
_shockwaveRenderer(),
_rc(),
_pixelToUnitRatio(1)
{
    // Empty
}

void Renderer::reset()
{
    _rc.reset();
    _pixelToUnitRatio = 1;
    
    _circleBatchers.clear();
    _fontBatchers.clear();
    _framebuffers.clear();
    _imageViews.clear();
    _matrices.clear();
    _rektangleBatchers.clear();
    _spriteBatchers.clear();
    _textViews.clear();
    _triangleBatchers.clear();
}

void Renderer::createDeviceDependentResources()
{
    _box2DPhysicsRenderer.createDeviceDependentResources();
    _nosPhysicsRenderer.createDeviceDependentResources();
    _shockwaveRenderer.createDeviceDependentResources();
    _lightRenderer.createDeviceDependentResources();
    _framebufferRenderer.createDeviceDependentResources();
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
    _box2DPhysicsRenderer.destroyDeviceDependentResources();
    _nosPhysicsRenderer.destroyDeviceDependentResources();
    _shockwaveRenderer.destroyDeviceDependentResources();
    _lightRenderer.destroyDeviceDependentResources();
    _framebufferRenderer.destroyDeviceDependentResources();
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
    configShader("sprite");
    
    clearFramebuffer(_rc.clearColor);
    
    if (!ASSETS_MGR.isShaderLoaded(_rc.shaderKey) ||
        !ASSETS_MGR.isFontLoaded(_rc.fontBatcherKey))
    {
        return;
    }
    
    std::string loadingText = "Loading";
    int numDots = stateTime % 32 / 8;
    for (int i = 0; i < numDots; ++i)
    {
        loadingText += ".";
    }
    
    Matrix& m = matrix(_rc.matrixKey);
    FontBatcher& fb = fontBatcher(_rc.fontBatcherKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    fb.begin();
    fb.addText(m, loadingText, TEXA_LEFT, 0.80f, 0.04f, 0.02f);
    fb.end(m, s);
}

void Renderer::bindFramebuffer(std::string framebufferKey)
{
    Framebuffer& fb = framebuffer(framebufferKey);
    OGL.bindFramebuffer(fb);
    OGL.enableBlending(_rc.enableBlending);
    
    _rc.framebufferKey = framebufferKey;
}

void Renderer::clearFramebuffer(Color clearColor)
{
    _rc.clearColor = clearColor;
    OGL.clearFramebuffer(_rc.clearColor);
}

void Renderer::renderImageViews()
{
    configShader("sprite");
    
    Matrix& m = matrix(_rc.matrixKey);
    float l = m._desc._left;
    float b = m._desc._bottom;
    float w = m._desc.width();
    float h = m._desc.height();
    for (auto& pair : _imageViews)
    {
        ImageView& iv = pair.second;
        renderSprite(iv._texture, iv._textureRegion, l + w * iv._xWeight, b + h * iv._yWeight, w * iv._widthWeight, h * iv._heightWeight, 0, 0, false);
    }
}

void Renderer::resetMatrix()
{
    Matrix& m = matrix(_rc.matrixKey);
    updateMatrix(m._base);
}

void Renderer::updateMatrix(float l, float r, float b, float t, float n, float f)
{
    Matrix& m = matrix(_rc.matrixKey);
    m.ortho(l, r, b, t, n, f);
}

void Renderer::updateMatrix(MatrixDescriptor& desc)
{
    updateMatrix(desc._left, desc._right, desc._bottom, desc._top, desc._near, desc._far);
}

void Renderer::updateMatrixCenteredOnPlayer()
{
    Entity* e = _rc.controlledPlayer;
    if (e == nullptr)
    {
        return;
    }
    
    Matrix& m = matrix(_rc.matrixKey);
    MatrixDescriptor md = m._base;
    float width = md.width() * _rc.scale;
    float height = md.height() * _rc.scale;
    md._right = CLAMP(e->position()._x + width / 2, 0, _rc.maxRight);
    md._top = CLAMP(e->position()._y + height / 2, 0, _rc.maxTop);
    
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
    
    updateMatrix(md);
}

void Renderer::updateMatrixCenteredOnPlayerForParallaxLayer(float parallaxSpeedRatio)
{
    updateMatrixCenteredOnPlayer();
    
    Matrix& m = matrix(_rc.matrixKey);
    MatrixDescriptor md = m._desc;
    
    float left = md._left * parallaxSpeedRatio;
    float bottom = md._bottom * parallaxSpeedRatio;
    
    md._right += (left - md._left);
    md._left = left;
    md._top += (bottom - md._bottom);
    md._bottom = bottom;
    
    updateMatrix(md);
}

void Renderer::rektangleBatcherBegin()
{
    rektangleBatcher(_rc.rektangleBatcherKey).begin();
}

void Renderer::rektangleBatcherAddRektangle(Rektangle& r)
{
    rektangleBatcher(_rc.rektangleBatcherKey).addRektangle(r);
}

void Renderer::rektangleBatcherEnd(Color& c)
{
    RektangleBatcher& rb = rektangleBatcher(_rc.rektangleBatcherKey);
    Matrix& m = matrix(_rc.matrixKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    
    rb.end(s, m._matrix, c);
}

void Renderer::renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, uint16_t stateTime, float angle, bool flipX)
{
    SpriteBatcher& sb = spriteBatcher(_rc.spriteBatcherKey);
    Matrix& m = matrix(_rc.matrixKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    Texture& t = ASSETS_MGR.texture(textureKey);
    TextureRegion& tr = ASSETS_MGR.textureRegion(textureRegionKey, stateTime);
    
    sb.begin();
    sb.addSprite(tr, x, y, width, height, angle, flipX);
    sb.end(s, m._matrix, t);
}

void Renderer::renderEntitiesBoundToTexture(std::string spriteBatcherKey, std::string texture, std::vector<Entity*>& entities)
{
    spriteBatcherBegin(spriteBatcherKey);
    for (Entity* e : entities)
    {
        if (e->isExiled())
        {
            continue;
        }
        std::string textureRegionKey = e->controllerRender()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        
        if (textureForRegionKey == texture)
        {
            spriteBatcherAddEntity(e);
        }
    }
    spriteBatcherEnd(spriteBatcherKey, texture);
}

void Renderer::renderRepeatingTextureParallaxLayers(std::string spriteBatcherKey, std::string texture, std::vector<Entity*>& layers)
{
    configSpriteBatcher(spriteBatcherKey);
    
    Matrix& m = matrix(_rc.matrixKey);
    SpriteBatcher& sb = spriteBatcher(_rc.spriteBatcherKey);
    
    sb.begin();
    for (Entity* e : layers)
    {
        std::string textureRegionKey = e->controllerRender()->getTextureMapping();
        std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
        
        if (textureForRegionKey == texture)
        {
            TextureRegion& trRaw = ASSETS_MGR.textureRegion(e->controllerRender()->getTextureMapping(),  e->stateTime());
            
            Texture& t = ASSETS_MGR.texture(texture);
            float parallaxSpeedRatio = e->data().getFloat("parallaxSpeedRatio");
            uint8_t gameToPixelRatio = t._width / e->width();
            uint16_t x = (m._desc._left * parallaxSpeedRatio) * gameToPixelRatio;
            x %= t._width;
            TextureRegion tr = trRaw.copyWithX(x);
            
            sb.addSprite(tr, e->position()._x + m._desc._left, e->position()._y, e->width(), e->height(), e->angle(), e->isXFlipped());
            
            float matrixWidthRemaining = m._desc.width() - e->width();
            for (int i = 1; matrixWidthRemaining > 0; matrixWidthRemaining -= e->width(), ++i)
            {
                x += t._width;
                x %= t._width;
                TextureRegion trX = tr.copyWithX(x);
                
                sb.addSprite(trX, e->position()._x + m._desc._left + e->width() * i, e->position()._y, e->width(), e->height(), e->angle(), e->isXFlipped());
            }
        }
    }
    spriteBatcherEnd(spriteBatcherKey, texture);
}

void Renderer::spriteBatcherBegin(std::string spriteBatcherKey)
{
    configSpriteBatcher(spriteBatcherKey);
    
    spriteBatcher(_rc.spriteBatcherKey).begin();
}

void Renderer::spriteBatcherAddEntities(std::vector<Entity*>& entities)
{
    SpriteBatcher& sb = spriteBatcher(_rc.spriteBatcherKey);
    for (Entity* e : entities)
    {
        assert(e != nullptr);
        
        if (e->isExiled())
        {
            continue;
        }
        
        spriteBatcherAddEntity(sb, *e);
    }
}

void Renderer::spriteBatcherAddEntity(Entity* e)
{
    assert(e != nullptr);
    
    SpriteBatcher& sb = spriteBatcher(_rc.spriteBatcherKey);
    spriteBatcherAddEntity(sb, *e);
}

void Renderer::spriteBatcherEnd(std::string spriteBatcherKey, std::string textureKey)
{
    configShader("sprite");
    configSpriteBatcher(spriteBatcherKey);
    
    SpriteBatcher& sb = spriteBatcher(_rc.spriteBatcherKey);
    Matrix& m = matrix(_rc.matrixKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    Texture& t = ASSETS_MGR.texture(textureKey);
    
    sb.end(s, m._matrix, t, _rc.spriteColorFactor);
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

void Renderer::renderTextViews()
{
    configShader("sprite");
    
    Matrix& m = matrix(_rc.matrixKey);
    FontBatcher& fb = fontBatcher(_rc.fontBatcherKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    fb.begin();
    for (auto& pair : _textViews)
    {
        fb.addText(m, pair.second);
    }
    fb.end(m, s, _rc.textColorFactor);
}

void Renderer::renderPhysics(World* world)
{
    bool isBox2D = _rc.physicsEngine == "Box2D";
    if (isBox2D)
    {
        renderBox2DPhysics(static_cast<Box2DPhysicsWorld*>(world));
    }
    else
    {
        renderNosPhysics(static_cast<NosPhysicsWorld*>(world));
    }
}

void Renderer::renderBox2DPhysics(Box2DPhysicsWorld* world)
{
    configShader("geometry");
    
    Matrix& m = matrix(_rc.matrixKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    _box2DPhysicsRenderer.render(world, &m._matrix, &s);
}

void Renderer::renderNosPhysics(NosPhysicsWorld* world)
{
    configShader("geometry");
    
    Matrix& m = matrix(_rc.matrixKey);
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    _nosPhysicsRenderer.render(world, &m._matrix, &s);
}

void Renderer::renderGameInfo(World &w)
{
    // TODO, this whole function should really be a shared Lua function,
    // since it directly references textViews from end user renderer.json
    
    configMatrix("hud");
    Matrix& m = matrix(_rc.matrixKey);
    updateMatrix(m._base);
    
    configShader("sprite");
    
    setTextVisible("rtt", true);
    setTextVisible("rollbackFrames", true);
    setTextVisible("inBps", true);
    setTextVisible("outBps", true);
    setTextVisible("player4Info", true);
    setTextVisible("player3Info", true);
    setTextVisible("player2Info", true);
    setTextVisible("player1Info", true);
    
    if (NW_CLNT)
    {
        setText("rtt", STRING_FORMAT("RTT %d ms", static_cast<int>(NW_CLNT->avgRoundTripTime())));
        setText("rollbackFrames", STRING_FORMAT("Rollback frames %d", _rc.numRollbackFrames));
        setText("inBps", STRING_FORMAT(" In %d Bps", static_cast<int>(NW_CLNT->bytesReceivedPerSecond())));
        setText("outBps", STRING_FORMAT("Out %d Bps", static_cast<int>(NW_CLNT->bytesSentPerSecond())));
    }
    
    Entity* player4 = w.getPlayer(4);
    if (player4 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(4) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            setText("player4Info", STRING_FORMAT("4|%s [.] to drop", player4->playerInfo()._playerName.c_str()));
        }
        else
        {
            setText("player4Info", STRING_FORMAT("4|%s", player4->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        setText("player4Info", "4|Maybe someone will join...?");
    }
    
    Entity* player3 = w.getPlayer(3);
    if (player3 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(3) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            setText("player3Info", STRING_FORMAT("3|%s [.] to drop", player3->playerInfo()._playerName.c_str()));
        }
        else
        {
            setText("player3Info", STRING_FORMAT("3|%s", player3->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        setText("player3Info", "3|Maybe someone will join...?");
    }
    
    Entity* player2 = w.getPlayer(2);
    if (player2 != nullptr)
    {
        if (NW_CLNT->isPlayerIDLocal(2) &&
            NW_CLNT->isPlayerIDLocal(1))
        {
            setText("player2Info", STRING_FORMAT("2|%s [.] to drop", player2->playerInfo()._playerName.c_str()));
        }
        else
        {
            setText("player2Info", STRING_FORMAT("2|%s", player2->playerInfo()._playerName.c_str()));
        }
    }
    else
    {
        setText("player2Info", "2|Connect gamepad or use arrow keys...");
    }
    
    Entity* player1 = w.getPlayer(1);
    if (player1 != nullptr)
    {
        setText("player1Info", STRING_FORMAT("1|%s", player1->playerInfo()._playerName.c_str()));
    }
    else
    {
        setText("player1Info", "1|Joining...");
    }
    
    setText("fps", STRING_FORMAT("FPS %d", FPS_UTIL.fps()));
    
    configTextColorFactor(Color::RED);
    renderTextViews();
}

void Renderer::renderLight(std::string framebufferKey, std::string framebufferNormalMapKey, float lightPosZ, std::vector<Entity*>& lights)
{
    Shader& s = ASSETS_MGR.shader("lights");
    Framebuffer& fb = framebuffer(framebufferKey);
    Framebuffer& fbNormalMap = framebuffer(framebufferNormalMapKey);
    Matrix& m = matrix(_rc.matrixKey);
    
    _lightRenderer.resetLights();
    _lightRenderer.configAmbientLight(0.0f, 0.0f, 0.0f, 0.0f);
    _lightRenderer.configureFallOff(0.3f, 0.3f, 20.0f);
    
    for (Entity* e : lights)
    {
        _lightRenderer.addLight(e->position()._x, e->position()._y, lightPosZ, 0.9f, 0.9f, 0.9f, 0.9f);
    }
    
    _lightRenderer.render(s, m, fb, fbNormalMap);
}

void Renderer::renderFramebufferWithShockwave(std::string framebufferKey, float centerX, float centerY, uint16_t timeElapsed, bool isTransforming)
{
    Shader& s = ASSETS_MGR.shader("shockwave");
    Framebuffer& fb = framebuffer(framebufferKey);
    Matrix& m = matrix(_rc.matrixKey);
    
    static float frameRate = static_cast<float>(ENGINE_CFG.frameRate());
    
    float realTimeElapsed = timeElapsed * frameRate;
    
    _shockwaveRenderer.render(s, fb, m, centerX, centerY, realTimeElapsed, isTransforming);
}

void Renderer::renderFramebuffer(std::string framebufferKey, std::string shaderKey)
{
    configShader(shaderKey);
    
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    Framebuffer& fb = framebuffer(framebufferKey);
    
    _framebufferRenderer.renderFramebuffer(s, fb);
}

void Renderer::renderFramebuffer(std::string framebufferKey)
{
    renderFramebuffer(framebufferKey, "framebuffer");
}

void Renderer::renderToScreen()
{
    configShader("framebuffer");
    
    Shader& s = ASSETS_MGR.shader(_rc.shaderKey);
    Framebuffer& fb = framebuffer(_rc.framebufferKey);
    _screenRenderer.renderToScreen(s, fb);
}

void Renderer::configReset()
{
    _rc.reset();
}

void Renderer::configBounds(uint32_t maxRight, uint32_t maxTop, float& scale)
{
    Matrix& m = matrix(_rc.matrixKey);
    
    const float baseRight = m._base._right;
    const float baseTop = m._base._top;
    float maxWidthScale = maxRight / baseRight;
    float maxHeightScale = maxTop / baseTop;
    float maxScale = CLAMP(GOW_MIN(maxWidthScale, maxHeightScale), 1.0f, 200.0f);
    
    _rc.maxRight = maxRight;
    _rc.maxTop = maxTop;
    scale = CLAMP(scale, 0.125f, maxScale);
    _rc.scale = scale;
}

void Renderer::configControlledPlayerEntity(Entity* e)
{
    _rc.controlledPlayer = e;
}

void Renderer::configMatrix(std::string key)
{
    _rc.matrixKey = key;
}

void Renderer::configShader(std::string key)
{
    _rc.shaderKey = key;
}

void Renderer::configSpriteBatcher(std::string key)
{
    _rc.spriteBatcherKey = key;
}

void Renderer::configSpriteColorFactor(Color c)
{
    _rc.spriteColorFactor = c;
}

void Renderer::configTextColorFactor(Color c)
{
    _rc.textColorFactor = c;
}

void Renderer::configPhysicsEngine(std::string physicsEngine)
{
    _rc.physicsEngine = physicsEngine;
}

void Renderer::configNumRollbackFrames(int numRollbackFrames)
{
    _rc.numRollbackFrames = numRollbackFrames;
}

Matrix& Renderer::matrixForInput()
{
    return matrix(_rc.matrixKey);
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
    TextureRegion& tr = ASSETS_MGR.textureRegion(e.controllerRender()->getTextureMapping(),  e.stateTime());
    // TODO, is scale the best way to handle this?
    // It adds 2 multiplication operations for every single entity rendered
    float scale = e.entityDef()._scale;
    assert(tr._width % _pixelToUnitRatio == 0);
    assert(tr._height % _pixelToUnitRatio == 0);
    float realWidth = tr._width / _pixelToUnitRatio * scale;
    float realHeight = tr._height / _pixelToUnitRatio * scale;
    sb.addSprite(tr, e.position()._x, e.position()._y, realWidth, realHeight, e.angle(), e.isXFlipped());
}
