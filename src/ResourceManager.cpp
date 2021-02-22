//
//  ResourceManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ResourceManager.hpp"

#include "Assets.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

#include <assert.h>

void ResourceManager::createDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        loadAssets(a);
    }
}

void ResourceManager::releaseDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        unloadAssets(a);
    }
}

void ResourceManager::registerAssets(std::string assetsFilePath)
{
    assert(_assets.find(assetsFilePath) == _assets.end());
    
    _assets.emplace(assetsFilePath, Assets{});
}

void ResourceManager::deregisterAssets(std::string assetsFilePath)
{
    auto q = _assets.find(assetsFilePath);
    assert(q != _assets.end());
    
    unloadAssets(q->second);
    _assets.erase(q);
}

Shader& ResourceManager::shader(std::string name)
{
    Shader* ret = NULL;
    for (auto& pair : _assets)
    {
        // TODO
    }
    
    return *ret;
}

Texture& ResourceManager::texture(std::string name)
{
    Texture* ret = NULL;
    for (auto& pair : _assets)
    {
        // TODO
    }
    
    return *ret;
}

TextureRegion& ResourceManager::findTextureRegion(std::string key, uint16_t stateTime)
{
    TextureRegion* ret = NULL;
    
    for (auto& pair : _assets)
    {
        ret = pair.second.findTextureRegion(key, stateTime);
        if (ret != NULL)
        {
            break;
        }
    }
    
    assert(ret != NULL);
    
    return *ret;
}

void ResourceManager::loadAssets(Assets& a)
{
    _shaderManager.loadShaders(a.getShaderDescriptors());
    _textureManager.loadTextures(a.getTextureDescriptors());
}

void ResourceManager::unloadAssets(Assets& a)
{
    _shaderManager.unloadShaders(a.getShaderDescriptors());
    _textureManager.unloadTextures(a.getTextureDescriptors());
}

ResourceManager::ResourceManager() :
_shaderManager(),
_textureManager()
{
    // Empty
}
