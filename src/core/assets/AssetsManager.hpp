//
//  AssetsManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/opengl/ShaderManager.hpp"
#include "core/audio/SoundManager.hpp"
#include "core/opengl/TextureManager.hpp"

#include "Assets.hpp"

#include <map>

#define ASSETS_MGR AssetsManager::getInstance()

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
    SoundWrapper* sound(std::string soundID);
    std::map<std::string, SoundWrapper*>& sounds();
    SoundWrapper* music();
    Texture& texture(std::string name);
    bool isTextureLoaded(std::string name);
    TextureRegion& textureRegion(std::string key, uint16_t stateTime = 0);
    bool isLoaded();

private:
    std::map<std::string, Assets> _assets;
    ShaderManager _shaderMgr;
    SoundManager _soundMgr;
    TextureManager _textureMgr;
    bool _isLoaded;
    
    bool areAssetsLoaded();
    
    AssetsManager();
    ~AssetsManager() {}
    AssetsManager(const AssetsManager&);
    AssetsManager& operator=(const AssetsManager&);
};
