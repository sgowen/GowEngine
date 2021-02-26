//
//  AssetsManager.hpp
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

#define ASSETS AssetsManager::getInstance()

class AssetsManager
{
public:
	static AssetsManager& getInstance()
    {
        static AssetsManager ret = AssetsManager();
        return ret;
    }
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void registerAssets(std::string key, Assets a);
    void deregisterAssets(std::string key);
    Shader& shader(std::string name);
    SoundWrapper* sound(uint16_t soundID);
    std::map<uint16_t, SoundWrapper*>& sounds();
    SoundWrapper* music();
    Texture& texture(std::string name);
    TextureRegion& textureRegion(std::string key, uint16_t stateTime = 0);

private:
    std::map<std::string, Assets> _assets;
    ShaderManager _shaderMgr;
    SoundManager _soundMgr;
    TextureManager _textureMgr;
    
    AssetsManager();
    ~AssetsManager() {}
    AssetsManager(const AssetsManager&);
    AssetsManager& operator=(const AssetsManager&);
};
