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
    return _shaderMgr.shader(name);
}

SoundWrapper* ResourceManager::sound(uint16_t soundID)
{
    return _soundMgr.sound(soundID);
}

Texture& ResourceManager::texture(std::string name)
{
    return _textureMgr.texture(name);
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
    _shaderMgr.loadShaders(a.getShaderDescriptors());
    _soundMgr.loadSounds(a.getSoundDescriptors());
    _textureMgr.loadTextures(a.getTextureDescriptors());
}

void ResourceManager::unloadAssets(Assets& a)
{
    _shaderMgr.unloadShaders(a.getShaderDescriptors());
    _soundMgr.unloadSounds(a.getSoundDescriptors());
    _textureMgr.unloadTextures(a.getTextureDescriptors());
}

ResourceManager::ResourceManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
