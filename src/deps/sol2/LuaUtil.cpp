//
//  LuaUtil.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/18/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

void LuaUtil::loadScript(Script& s)
{
    if (ENGINE_CFG.logLua())
    {
        LOG("LuaUtil::loadScript %s", s._desc._filePath.c_str());
    }
    
    assert(s._fileData != nullptr && s._fileData->_length > 0);
    
    s._lua = new sol::state();
    sol::state& lua = *s._lua;
    sol::load_result loadedScript = lua.load_buffer((const char*)s._fileData->_data, s._fileData->_length);
    sol::protected_function_result result = loadedScript();
    if (!result.valid())
    {
        LOG("LuaUtil: %s is not valid", s._desc._name.c_str());
    }
    assert(result.valid());
    
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string);
    
    if (ENGINE_CFG.logLua())
    {
        lua.set_function("LOG", [](std::string line) {
            LOG(line.c_str());
        });
    }
    
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
}

void LuaUtil::unloadScript(Script& s)
{
    if (ENGINE_CFG.logLua())
    {
        LOG("LuaUtil::unloadScript %s", s._desc._filePath.c_str());
    }
    
    assert(s._lua != nullptr);
    
    delete s._lua;
    s._lua = nullptr;
}

void LuaUtil::bindToEntity(sol::state& lua, Entity& e)
{
    Vector2& vel = e.velocity();
    uint8_t& stateFlags = e.state()._stateFlags;
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e.networkDataField("health").setValueUInt8(value);
    });
    
    lua.set_function("state", [&e]() {
        uint8_t stateVal = e.state()._state;
        return e.state(stateVal);
    });
    
    lua.set_function("stateTime", [&e]() {
        uint16_t stateTimeVal = e.state()._stateTime;
        return stateTimeVal;
    });
    
    lua.set_function("getFloat", [&e](std::string key) {
        return e.data().getFloat(key);
    });
    
    lua.set_function("setState", [&e](std::string key) {
        uint8_t& state = e.state()._state;
        uint16_t& stateTime = e.state()._stateTime;
        
        state = e.state(key);
        stateTime = 0;
    });
    
    lua.set_function("isGrounded", [&e]() {
        return e.isGrounded();
    });
    
    lua.set_function("setVelocityX", [&e](float x) {
        Vector2& vel = e.velocity();
        vel._x = x;
    });
    
    lua.set_function("setVelocityY", [&e](float y) {
        Vector2& vel = e.velocity();
        vel._y = y;
    });
    
    lua.set_function("getVelocityY", [&e]() {
        Vector2& vel = e.velocity();
        return vel._y;
    });
    
    lua.set_function("setXFlipped", [&e](bool value) {
        e.pose()._isXFlipped = value;
    });
    
    lua.set_function("isXFlipped", [&e]() {
        return e.pose()._isXFlipped;
    });
    
    lua.set_function("getNetworkUInt8", [&e](std::string name) {
        return e.networkDataField(name).valueUInt8();
    });
    
    lua.set_function("getUInt", [&e](std::string key) {
        return e.data().getUInt(key);
    });
    
    lua.set_function("exile", [&e]() {
        e.exile();
    });
    
    lua.set_function("getNetworkUInt32", [&e](std::string name) {
        return e.networkDataField(name).valueUInt32();
    });
    
    lua.set_function("getNetworkUInt16", [&e](std::string name) {
        return e.networkDataField(name).valueUInt16();
    });
    
    lua.set_function("sendMessage", [&e](uint32_t entityID, std::string message) {
        Entity* entity = e.world()->getEntityByID(entityID);
        if (entity != nullptr)
        {
            entity->message(e.message(message), &e);
        }
    });
    
    lua.set_function("isPlayer", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        bool ret = entity ? entity->isPlayer() : false;
        
        return ret;
    });
    
    lua.set_function("setNetworkUInt32", [&e](std::string key, uint32_t value) {
        e.networkDataField(key).setValueUInt32(value);
    });
    
    lua.set_function("setNetworkUInt16", [&e](std::string key, uint16_t value) {
        e.networkDataField(key).setValueUInt16(value);
    });
    
    lua.set_function("setZeroGravity", [&e](bool value) {
        e.pose()._isZeroGravity = value;
    });
    
    lua.set_function("setSlowed", [&e](bool value) {
        e.pose()._isSlowed = value;
    });
    
    lua.set_function("setNetworkUInt8", [&e](std::string key, uint8_t value) {
        e.networkDataField(key).setValueUInt8(value);
    });
    
    lua.set_function("setNetworkBool", [&e](std::string key, bool value) {
        e.networkDataField(key).setValueBool(value);
    });
    
    lua.set_function("getNetworkBool", [&e](std::string key) {
        return e.networkDataField(key).valueBool();
    });
    
    lua.set_function("setStateFlag", [&e, &stateFlags](std::string stateFlag, bool value) {
        uint16_t flag = e.stateFlag(stateFlag);
        SET_BIT(stateFlags, flag, value);
    });
    
    lua.set_function("isStateFlagSet", [&e, &stateFlags](std::string key) {
        bool ret = IS_BIT_SET(stateFlags, e.stateFlag(key));
        
        return ret;
    });
    
    lua.set_function("resetVelocity", [&vel]() {
        vel.reset();
    });
    
    lua.set_function("findTargetWithinDistance", [&e](float shortestDistance) {
        Entity* target = nullptr;
        for (Entity* p : e.world()->getPlayers())
        {
            float distance = p->position().dist(e.position());
            if (distance < shortestDistance)
            {
                shortestDistance = distance;
                target = p;
            }
        }
        
        uint32_t ret = target ? target->getID() : 0;
        
        return ret;
    });
    
    lua.set_function("entityPositionX", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        return entity ? entity->position()._x : 0;
    });
    
    lua.set_function("entityPositionY", [&e](uint32_t entityID) {
        Entity* entity = e.world()->getEntityByID(entityID);
        
        return entity ? entity->position()._y : 0;
    });
    
    lua.set_function("positionX", [&e]() {
        return e.position()._x;
    });
    
    lua.set_function("positionY", [&e]() {
        return e.position()._y;
    });
    
    lua.set_function("wasInputFlagSet", [&e](std::string key) {
        uint16_t flag = e.inputStateFlag(key);
        bool ret = IS_BIT_SET(e.lastProcessedInputState(), flag);
        
        return ret;
    });
}

void LuaUtil::bindToRenderer(sol::state& lua, Renderer& r)
{
    std::map<std::string, Color>& colorMap = _colorMap;
    lua.set_function("configSpriteColorFactor", [&r, &colorMap](std::string key) {
        Color colorFactor = colorMap[key];
        r.configSpriteColorFactor(colorFactor);
    });
    
    lua.set_function("configTextColorFactor", [&r, &colorMap](std::string key) {
        Color colorFactor = colorMap[key];
        r.configTextColorFactor(colorFactor);
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
    
    lua.set_function("renderFramebufferToScreen", [&r](std::string framebufferKey) {
        r.renderFramebufferToScreen(framebufferKey);
    });
    
    lua.set_function("spriteBatcherBegin", [&r](std::string spriteBatcherKey) {
        r.spriteBatcherBegin(spriteBatcherKey);
    });
    
    lua.set_function("spriteBatcherEnd", [&r](std::string spriteBatcherKey, std::string texture) {
        r.spriteBatcherEnd(spriteBatcherKey, texture);
    });
}

void LuaUtil::bindToWorld(sol::state& lua, World& w)
{
    lua.set_function("doesPlayerExist", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity != nullptr && !entity->isExiled();
    });
    
    lua.set_function("playerPositionX", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity ? entity->position()._x : 0;
    });
    
    lua.set_function("playerPositionY", [&w](int playerID) {
        Entity* entity = w.getPlayer(playerID);
        
        return entity ? entity->position()._y : 0;
    });
    
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
}

void LuaUtil::bindToRendererAndWorld(sol::state& lua, Renderer& r, World& w)
{
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
}

LuaUtil::LuaUtil()
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
}
