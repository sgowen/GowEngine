//
//  AssetManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AssetManager.hpp"

#include "Assets.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

#include <assert.h>

void AssetManager::createDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.loadShaders(a.getShaderDescriptors());
        _soundMgr.loadSounds(a.getSoundDescriptors());
        _textureMgr.loadTextures(a.getTextureDescriptors());
    }
}

void AssetManager::releaseDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.unloadShaders(a.getShaderDescriptors());
        _soundMgr.unloadSounds(a.getSoundDescriptors());
        _textureMgr.unloadTextures(a.getTextureDescriptors());
    }
}

void AssetManager::registerAssets(std::string assetsFilePath)
{
    assert(_assets.find(assetsFilePath) == _assets.end());
    _assets.emplace(assetsFilePath, Assets{});
}

void AssetManager::deregisterAssets(std::string assetsFilePath)
{
    auto q = _assets.find(assetsFilePath);
    assert(q != _assets.end());
    _assets.erase(q);
}

Shader& AssetManager::shader(std::string name)
{
    return _shaderMgr.shader(name);
}

SoundWrapper* AssetManager::sound(uint16_t soundID)
{
    return _soundMgr.sound(soundID);
}

SoundWrapper* AssetManager::music()
{
    return _soundMgr.music();
}

Texture& AssetManager::texture(std::string name)
{
    return _textureMgr.texture(name);
}

TextureRegion& AssetManager::findTextureRegion(std::string key, uint16_t stateTime)
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

AssetManager::AssetManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
