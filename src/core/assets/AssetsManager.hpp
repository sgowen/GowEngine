//
//  AssetsManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/Font.hpp"
#include "core/opengl/ShaderManager.hpp"
#include "core/openal/SoundManager.hpp"
#include "core/opengl/TextureManager.hpp"

#include "Assets.hpp"

#include <map>

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
    ShaderManager _shaderMgr;
    SoundManager _soundMgr;
    TextureManager _textureMgr;
    uint32_t _stateTime;
    bool _isLoadingData;
    bool _isLoadedIntoEngine;
    
    void beginLoad();
    
    AssetsManager();
    ~AssetsManager() {}
    AssetsManager(const AssetsManager&);
    AssetsManager& operator=(const AssetsManager&);
};
