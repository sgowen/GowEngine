//
//  LuaRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

LuaRenderer::LuaRenderer(Renderer& r) : _lua(new sol::state())
{
    _colorMap.emplace("CLEAR", Color::CLEAR);
    _colorMap.emplace("WHITE", Color::WHITE);
    _colorMap.emplace("BLACK", Color::BLACK);
    _colorMap.emplace("RED", Color::RED);
    _colorMap.emplace("GREEN", Color::GREEN);
    _colorMap.emplace("BLUE", Color::BLUE);
    _colorMap.emplace("HALF", Color::HALF);
    _colorMap.emplace("DIM", Color::DIM);
    _colorMap.emplace("DOUBLE", Color::DOUBLE);
    _colorMap.emplace("ONE", Color::ONE);
    
    sol::state& lua = *_lua;
    
    lua.open_libraries(sol::lib::base);
    
    lua.set_function("log", [](std::string line) {
        LOG(line.c_str());
    });
    
    lua.set_function("updateMatrixCenteredOnPlayerForParallaxLayer", [&r](float parallaxSpeedRatio) {
        r.updateMatrixCenteredOnPlayerForParallaxLayer(parallaxSpeedRatio);
    });
    
    lua.set_function("updateMatrixCenteredOnPlayer", [&r] {
        r.updateMatrixCenteredOnPlayer();
    });
    
    lua.set_function("bindFramebuffer", [&r](std::string framebufferKey) {
        r.bindFramebuffer(framebufferKey);
    });
    
    lua.set_function("renderFramebuffer", [&r](std::string framebufferKey) {
        r.renderFramebuffer(framebufferKey);
    });
    
    lua.set_function("renderFramebuffer", [&r](std::string framebufferKey, std::string shaderKey) {
        r.renderFramebuffer(framebufferKey, shaderKey);
    });
    
    lua.set_function("renderFramebufferWithShockwave", [&r](std::string framebufferKey, float centerX, float centerY, uint16_t timeElapsed, bool isTransforming) {
        r.renderFramebufferWithShockwave(framebufferKey, centerX, centerY, timeElapsed, isTransforming);
    });
    
    lua.set_function("clearFramebuffer", [&r] {
        r.clearFramebuffer(Color::GREEN);
    });
}

LuaRenderer::~LuaRenderer()
{
    delete _lua;
    _lua = nullptr;
}

void LuaRenderer::render(Renderer& r, World& w, std::string script)
{
    sol::state& lua = *_lua;
    
    lua.set_function("isReleasingShockwave", [&w](int playerID) {
        Entity* jon = w.getPlayer(playerID);
        
        if (jon != nullptr && !jon->isExiled())
        {
            bool isReleasingShockwave = jon->networkDataField("isReleasingShockwave").valueBool();
            return isReleasingShockwave;
        }
        
        return false;
    });
    
    lua.set_function("stateStr", [&w](int playerID) {
        Entity* jon = w.getPlayer(playerID);
        
        std::string ret;
        if (jon != nullptr && !jon->isExiled())
        {
            uint8_t stateVal = jon->state()._state;
            ret = jon->state(stateVal);
        }
        
        return ret;
    });
    
    lua.set_function("shockwaveStateTime", [&w](int playerID) {
        uint16_t ret = 0;
        Entity* jon = w.getPlayer(playerID);
        
        if (jon != nullptr && !jon->isExiled())
        {
            uint16_t shockwaveStartTime = jon->networkDataField("shockwaveStateTime").valueUInt16();
            ret = shockwaveStartTime;
        }
        
        return ret;
    });
    
    lua.set_function("renderLayersBoundToTexture", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderEntitiesBoundToTexture(spriteBatcherKey, texture, w.getLayers());
    });
    
    lua.set_function("renderDynamicEntitiesBoundToTexture", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderEntitiesBoundToTexture(spriteBatcherKey, texture, w.getDynamicEntities());
    });
    
    lua.set_function("renderStaticEntitiesBoundToTexture", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderEntitiesBoundToTexture(spriteBatcherKey, texture, w.getStaticEntities());
    });
    
    lua.set_function("renderPlayersBoundToTexture", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderEntitiesBoundToTexture(spriteBatcherKey, texture, w.getPlayers());
    });
    
    lua.set_function("renderRepeatingTextureParallaxLayers", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderRepeatingTextureParallaxLayers(spriteBatcherKey, texture, w.getLayers());
    });
    
    lua.set_function("renderLight", [&r, &w](std::string framebufferKey, std::string framebufferNormalMapKey, float lightPosZ) {
        r.renderLight(framebufferKey, framebufferNormalMapKey, lightPosZ, w.getPlayers());
    });
    
    std::map<std::string, Color>& colorMap = _colorMap;
    lua.set_function("configSpriteColorFactor", [&r, &colorMap](std::string colorFactorKey) {
        Color colorFactor = colorMap[colorFactorKey];
        r.configSpriteColorFactor(colorFactor);
    });
    
    lua.set_function("spriteBatcherBegin", [&r, &w](std::string spriteBatcherKey) {
        r.spriteBatcherBegin(spriteBatcherKey);
    });
    
    lua.set_function("spriteBatcherAddEntity", [&r, &w](int entityID) {
        Entity* entity = w.getEntityByID(entityID);
        
        if (entity != nullptr && !entity->isExiled())
        {
            r.spriteBatcherAddEntity(entity);
        }
    });
    
    lua.set_function("spriteBatcherAddPlayer", [&r, &w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        if (entity != nullptr && !entity->isExiled())
        {
            r.spriteBatcherAddEntity(entity);
        }
    });
    
    lua.set_function("spriteBatcherEnd", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.spriteBatcherEnd(spriteBatcherKey, texture);
    });
    
    lua.set_function("textureNameForPlayer", [&r, &w](int playerID) {
        std::string ret;
        
        Entity* entity = w.getPlayer(playerID);
        if (entity != nullptr && !entity->isExiled())
        {
            std::string textureRegionKey = entity->controllerRender()->getTextureMapping();
            std::string textureForRegionKey = ASSETS_MGR.textureForRegionKey(textureRegionKey);
            ret = textureForRegionKey;
        }
        
        return ret;
    });
    
    lua.set_function("playerPositionX", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity ? entity->position()._x : 0;
    });
    
    lua.set_function("playerPositionY", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity ? entity->position()._y : 0;
    });
    
    Script& s = ASSETS_MGR.script(script);
    
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid() && ENGINE_CFG.logLua())
    {
        LOG("LuaRenderer: %s is not valid", s._desc._name.c_str());
    }
}
