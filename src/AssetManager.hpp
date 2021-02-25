//
//  AssetManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "TextureDescriptor.hpp"
#include "Assets.hpp"
#include "ShaderManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

#include <map>

#define ASSETS AssetManager::getInstance()

class AssetManager
{
public:
	static AssetManager& getInstance()
    {
        static AssetManager ret = AssetManager();
        return ret;
    }
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void registerAssets(std::string assetsFilePath);
    void deregisterAssets(std::string assetsFilePath);
    Shader& shader(std::string name);
    SoundWrapper* sound(uint16_t soundID);
    SoundWrapper* music();
    Texture& texture(std::string name);
    TextureRegion& findTextureRegion(std::string key, uint16_t stateTime = 0);

private:
    std::map<std::string, Assets> _assets;
    ShaderManager _shaderMgr;
    SoundManager _soundMgr;
    TextureManager _textureMgr;
    
    AssetManager();
    ~AssetManager() {}
    AssetManager(const AssetManager&);
    AssetManager& operator=(const AssetManager&);
};
