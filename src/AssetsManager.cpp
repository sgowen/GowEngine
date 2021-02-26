//
//  AssetsManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "AssetsManager.hpp"

#include "Assets.hpp"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"

#include <assert.h>

void AssetsManager::createDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.loadShaders(a._shaderDescriptors);
        _soundMgr.loadSounds(a._soundDescriptors);
        _textureMgr.loadTextures(a._textureDescriptors);
    }
}

void AssetsManager::releaseDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.unloadShaders(a._shaderDescriptors);
        _soundMgr.unloadSounds(a._soundDescriptors);
        _textureMgr.unloadTextures(a._textureDescriptors);
    }
}

void AssetsManager::registerAssets(std::string key, Assets a)
{
    assert(_assets.find(key) == _assets.end());
    _assets.emplace(key, a);
}

void AssetsManager::deregisterAssets(std::string key)
{
    auto q = _assets.find(key);
    assert(q != _assets.end());
    _assets.erase(q);
}

Shader& AssetsManager::shader(std::string name)
{
    return _shaderMgr.shader(name);
}

SoundWrapper* AssetsManager::sound(uint16_t soundID)
{
    return _soundMgr.sound(soundID);
}

std::map<uint16_t, SoundWrapper*>& AssetsManager::sounds()
{
    return _soundMgr.sounds();
}

SoundWrapper* AssetsManager::music()
{
    return _soundMgr.music();
}

Texture& AssetsManager::texture(std::string name)
{
    return _textureMgr.texture(name);
}

TextureRegion& AssetsManager::textureRegion(std::string key, uint16_t stateTime)
{
    TextureRegion* ret = NULL;
    
    for (auto& pair : _assets)
    {
        ret = pair.second.textureRegion(key, stateTime);
        if (ret != NULL)
        {
            break;
        }
    }
    
    assert(ret != NULL);
    
    return *ret;
}

AssetsManager::AssetsManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
