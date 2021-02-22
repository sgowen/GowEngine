//
//  ResourceManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "TextureDescriptor.hpp"
#include "Assets.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

#include <map>

#define RES_MGR ResourceManager::getInstance()

class ResourceManager
{
public:
	static ResourceManager& getInstance()
    {
        static ResourceManager ret = ResourceManager();
        return ret;
    }
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void registerAssets(std::string assetsFilePath);
    void deregisterAssets(std::string assetsFilePath);
    Shader& shader(std::string name);
    Texture& texture(std::string name);
    TextureRegion& findTextureRegion(std::string key, uint16_t stateTime);

private:
    std::map<std::string, Assets> _assets;
    ShaderManager _shaderManager;
    TextureManager _textureManager;
    
    void loadAssets(Assets& a);
    void unloadAssets(Assets& a);
    
    ResourceManager();
    ~ResourceManager() {}
    ResourceManager(const ResourceManager&);
    ResourceManager& operator=(const ResourceManager&);
};
