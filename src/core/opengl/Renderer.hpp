//
//  Renderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/14/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ImageView.hpp"
#include "core/assets/TextView.hpp"
#include "core/physics/NosPhysicsRenderer.hpp"

#include "CircleBatcher.hpp"
#include "FontBatcher.hpp"
#include "Framebuffer.hpp"
#include "Matrix.hpp"
#include "RektangleBatcher.hpp"
#include "SpriteBatcher.hpp"
#include "TriangleBatcher.hpp"
#include "ShockwaveRenderer.hpp"
#include "ScreenRenderer.hpp"
#include "FramebufferRenderer.hpp"
#include "LightRenderer.hpp"

#include <map>

class Entity;
class Box2DPhysicsWorld;
class NosPhysicsWorld;

struct RendererConfig
{
    std::string fontBatcherKey;
    std::string framebufferKey;
    std::string matrixKey;
    std::string rektangleBatcherKey;
    std::string shaderKey;
    std::string spriteBatcherKey;
    uint32_t maxRight;
    uint32_t maxTop;
    float scale;
    bool enableBlending;
    Color clearColor;
    Color spriteColorFactor;
    Color textColorFactor;
    float parallaxSpeedRatio;
    Entity* controlledPlayer;
    std::string physicsEngine;
    bool physicsRenderingEnabled;
    int numRollbackFrames;
    
    RendererConfig()
    {
        reset();
    }
    
    void reset()
    {
        fontBatcherKey = "main";
        framebufferKey = "main";
        matrixKey = "main";
        rektangleBatcherKey = "main";
        shaderKey = "sprite";
        spriteBatcherKey = "main";
        maxRight = 32;
        maxTop = 32;
        scale = 1.0f;
        enableBlending = true;
        clearColor = Color::CLEAR;
        spriteColorFactor = Color::ONE;
        textColorFactor = Color::ONE;
        parallaxSpeedRatio = 1.0f;
        controlledPlayer = nullptr;
        physicsEngine = "";
        physicsRenderingEnabled = false;
        numRollbackFrames = 0;
    }
};

class Renderer
{
    friend class RendererLoader;
    friend class LuaRenderer;
    
public:
    Renderer();
    
    void reset();
    
    void createDeviceDependentResources();
    void onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight);
    void destroyDeviceDependentResources();
    
    void renderLoadingView(uint32_t stateTime);
    
    void bindFramebuffer(std::string framebufferKey);
    void clearFramebuffer(Color clearColor);
    
    void renderImageViews();
    
    void useMatrix(std::string key);
    void resetMatrix();
    void updateMatrix(float l, float r, float b, float t, float n, float f);
    void updateMatrix(MatrixDescriptor& desc);
    void updateMatrixCenteredOnPlayer();
    void updateMatrixCenteredOnPlayerForParallaxLayer(float parallaxSpeedRatio);
    
    void rektangleBatcherBegin();
    void rektangleBatcherAddRektangle(Rektangle& r);
    void rektangleBatcherEnd(Color& c);
    
    void renderSprite(std::string textureKey, std::string textureRegionKey, float x, float y, float width, float height, uint16_t stateTime, float angle, bool flipX);
    void renderEntitiesBoundToTexture(std::string spriteBatcherKey, std::string texture, std::vector<Entity*>& entities);
    void renderParallaxLayersBoundToRepeatingTexture(std::string spriteBatcherKey, std::string texture, std::vector<Entity*>& layers);
    void spriteBatcherBegin(std::string spriteBatcherKey);
    void spriteBatcherAddEntities(std::vector<Entity*>& entities);
    void spriteBatcherAddEntity(Entity* e);
    void spriteBatcherEnd(std::string spriteBatcherKey, std::string textureKey);
    
    void setTextVisible(std::string textViewKey, bool isVisible);
    void setText(std::string textViewKey, std::string text);
    void hideAllText();
    void renderTextViews();
    
    void renderPhysicsIfEnabled(World* world);
    void renderBox2DPhysics(Box2DPhysicsWorld* world);
    void renderNosPhysics(NosPhysicsWorld* world);
    
    void renderLight(std::string framebufferKey, std::string framebufferNormalMapKey, float lightPosZ, std::vector<Entity*>& entities);
    
    void renderFramebufferWithShockwave(std::string framebufferKey, float centerX, float centerY, uint16_t timeElapsed, bool isTransforming);
    void renderFramebuffer(std::string framebufferKey, std::string shaderKey);
    void renderFramebuffer(std::string framebufferKey);
    
    void renderToScreen();
    
    float clampScale();
    
    void configReset();
    void configBounds(uint32_t maxRight, uint32_t maxTop, float& scale);
    void configControlledPlayerEntity(Entity* e);
    void configMatrix(std::string key);
    void configShader(std::string key);
    void configSpriteBatcher(std::string key);
    void configSpriteColorFactor(Color c);
    void configTextColorFactor(Color c);
    void configPhysicsEngine(std::string physicsEngine);
    void configPhysicsRenderingEnabled(bool value);
    void configNumRollbackFrames(int numRollbackFrames);
    
    Matrix& matrixForInput();
    
private:
    Box2DPhysicsRenderer _box2DPhysicsRenderer;
    NosPhysicsRenderer _nosPhysicsRenderer;
    FramebufferRenderer _framebufferRenderer;
    LightRenderer _lightRenderer;
    ScreenRenderer _screenRenderer;
    ShockwaveRenderer _shockwaveRenderer;
    RendererConfig _rc;
    uint8_t _pixelToUnitRatio;
    std::map<std::string, CircleBatcher> _circleBatchers;
    std::map<std::string, FontBatcher> _fontBatchers;
    std::map<std::string, Framebuffer> _framebuffers;
    std::map<std::string, ImageView> _imageViews;
    std::map<std::string, Matrix> _matrices;
    std::map<std::string, RektangleBatcher> _rektangleBatchers;
    std::map<std::string, SpriteBatcher> _spriteBatchers;
    std::map<std::string, TextView> _textViews;
    std::map<std::string, TriangleBatcher> _triangleBatchers;
    
    void spriteBatcherAddEntity(SpriteBatcher& sb, Entity& e);
    
    CircleBatcher& circleBatcher(std::string key);
    FontBatcher& fontBatcher(std::string key);
    Framebuffer& framebuffer(std::string key);
    ImageView& imageView(std::string key);
    Matrix& matrix(std::string key);
    RektangleBatcher& rektangleBatcher(std::string key);
    SpriteBatcher& spriteBatcher(std::string key);
    TextView& textView(std::string key);
    TriangleBatcher& triangleBatcher(std::string key);
};
