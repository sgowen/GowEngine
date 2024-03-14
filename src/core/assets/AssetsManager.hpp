//
//  AssetsManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ScriptManager.hpp"
#include "core/opengl/Renderer.hpp"
#include "core/opengl/ShaderManager.hpp"
#include "core/openal/SoundManager.hpp"
#include "core/opengl/TextureManager.hpp"
#include "core/entity/EntityInputMappingManager.hpp"

#include "Assets.hpp"

#include <map>

struct Font;

class AssetsManager
{
public:
	static AssetsManager& getInstance()
    {
        static AssetsManager ret = AssetsManager();
        return ret;
    }
    
    void registerAssets(std::string key, Assets a);
    void deregisterAssets(std::string key);
    void update();
    void createDeviceDependentResourcesAsync();
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    
    Entity* createEntity(EntityInstanceDef eid);
    Entity* createEntity(EntityDef& ed, EntityInstanceDef eid);
    EntityDef& getEntityDef(uint32_t fourCCName);
    EntityInputMapping& entityInputMapping(std::string key);
    EntityLayout& entityLayout(uint32_t key);
    uint32_t getFirstLayout();
    Script& script(std::string name);
    Shader& shader(std::string name);
    bool isShaderLoaded(std::string name);
    Sound& sound(std::string soundID);
    std::map<std::string, Sound>& sounds();
    bool areSoundsLoaded();
    Font& font(std::string name);
    bool isFontLoaded(std::string name);
    Texture& texture(std::string name);
    bool isTextureLoaded(std::string name);
    std::string textureForRegionKey(std::string key);
    TextureRegion& textureRegion(std::string key, uint16_t stateTime = 0);
    Animation* animation(std::string key);
    uint32_t getStateTime();
    bool isLoaded();

private:
    std::map<std::string, Assets> _assets;
    std::map<uint32_t, EntityDef> _entityDefs;
    std::map<std::string, EntityInputMapping> _entityInputMappings;
    std::map<uint32_t, EntityLayout> _entityLayouts;
    ScriptManager _scriptMgr;
    ShaderManager _shaderMgr;
    SoundManager _soundMgr;
    TextureManager _textureMgr;
    Renderer _renderer;
    uint32_t _stateTime;
    
    AssetsManager();
    ~AssetsManager() {}
    AssetsManager(const AssetsManager&);
    AssetsManager& operator=(const AssetsManager&);
};
