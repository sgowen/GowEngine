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
    
    lua.open_libraries(sol::lib::base, sol::lib::string);
    
    lua.set_function("LOG", [](std::string line) {
        if (ENGINE_CFG.logLua())
        {
            LOG(line.c_str());
        }
    });
    
    lua.set_function("networkIsPlayerIDLocal", [](int playerID) {
        bool ret = false;
        if (NW_CLNT)
        {
            ret = NW_CLNT->isPlayerIDLocal(playerID);
        }
        return ret;
    });
    
    lua.set_function("networkAvgRoundTripTime", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->avgRoundTripTime());
        }
        return ret;
    });
    
    lua.set_function("networkBytesReceivedPerSecond", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->bytesReceivedPerSecond());
        }
        return ret;
    });
    
    lua.set_function("networkBytesSentPerSecond", []() {
        int ret = 0;
        if (NW_CLNT)
        {
            ret = static_cast<int>(NW_CLNT->bytesSentPerSecond());
        }
        return ret;
    });
    
    lua.set_function("fps", []() {
        return FPS_UTIL.fps();
    });
    
    lua.set_function("numRollbackFrames", [&r]() {
        return r._rc.numRollbackFrames;
    });
    
    lua.set_function("useMatrix", [&r](std::string key) {
        r.useMatrix(key);
    });
    
    lua.set_function("configShader", [&r](std::string key) {
        r.configShader(key);
    });
    
    lua.set_function("setTextVisible", [&r](std::string key, bool value) {
        r.setTextVisible(key, value);
    });
    
    lua.set_function("setText", [&r](std::string key, std::string value) {
        r.setText(key, value);
    });
    
    lua.set_function("renderTextViews", [&r]() {
        return r.renderTextViews();
    });
    
    std::map<std::string, Color>& colorMap = _colorMap;
    lua.set_function("configSpriteColorFactor", [&r, &colorMap](std::string key) {
        Color colorFactor = colorMap[key];
        r.configSpriteColorFactor(colorFactor);
    });
    
    lua.set_function("configTextColorFactor", [&r, &colorMap](std::string key) {
        Color colorFactor = colorMap[key];
        r.configTextColorFactor(colorFactor);
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
    
    lua.set_function("renderFramebuffer", sol::overload([&r](std::string framebufferKey) {
        r.renderFramebuffer(framebufferKey);
    }, [&r](std::string framebufferKey, std::string shaderKey) {
        r.renderFramebuffer(framebufferKey, shaderKey);
    }));
    
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
    
    lua.set_function("playerNetworkBool", [&w](int playerID, std::string name) {
        Entity* entity = w.getPlayer(playerID);
        
        bool ret = false;
        if (entity != nullptr)
        {
            ret = entity->networkDataField(name).valueBool();
        }
        
        return ret;
    });
    
    lua.set_function("playerNetworkUInt16", [&w](int playerID, std::string name) {
        Entity* entity = w.getPlayer(playerID);
        
        uint16_t ret = 0;
        if (entity != nullptr)
        {
            ret = entity->networkDataField(name).valueUInt16();
        }
        
        return ret;
    });
    
    lua.set_function("playerState", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        std::string ret;
        if (entity != nullptr && !entity->isExiled())
        {
            uint8_t stateVal = entity->state()._state;
            ret = entity->state(stateVal);
        }
        
        return ret;
    });
    
    lua.set_function("playerName", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        std::string ret;
        if (entity != nullptr && !entity->isExiled())
        {
            ret = entity->playerInfo()._playerName;
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
    
    lua.set_function("renderParallaxLayersBoundToRepeatingTexture", [&r, &w](std::string spriteBatcherKey, std::string texture) {
        r.renderParallaxLayersBoundToRepeatingTexture(spriteBatcherKey, texture, w.getLayers());
    });
    
    lua.set_function("renderPlayerLights", [&r, &w](std::string framebufferKey, std::string framebufferNormalMapKey, float lightPosZ) {
        r.renderLight(framebufferKey, framebufferNormalMapKey, lightPosZ, w.getPlayers());
    });
    
    lua.set_function("renderPhysicsIfEnabled", [&r, &w]() {
        r.renderPhysicsIfEnabled(&w);
    });
    
    lua.set_function("renderToScreen", [&r]() {
        r.renderToScreen();
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
    
    lua.set_function("doesPlayerExist", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity != nullptr && !entity->isExiled();
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
