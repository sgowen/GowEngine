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
        _shaderMgr.loadShaders(a._shaderDescriptors);
        _soundMgr.loadSounds(a._soundDescriptors);
        _textureMgr.loadTextures(a._textureDescriptors);
    }
}

void AssetManager::releaseDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.unloadShaders(a._shaderDescriptors);
        _soundMgr.unloadSounds(a._soundDescriptors);
        _textureMgr.unloadTextures(a._textureDescriptors);
    }
}

void AssetManager::registerAssets(std::string key, Assets a)
{
    assert(_assets.find(key) == _assets.end());
    _assets.emplace(key, a);
}

void AssetManager::deregisterAssets(std::string key)
{
    auto q = _assets.find(key);
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

std::map<uint16_t, SoundWrapper*>& AssetManager::sounds()
{
    return _soundMgr.sounds();
}

SoundWrapper* AssetManager::music()
{
    return _soundMgr.music();
}

Texture& AssetManager::texture(std::string name)
{
    return _textureMgr.texture(name);
}

TextureRegion& AssetManager::textureRegion(std::string key, uint16_t stateTime)
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

AssetManager::AssetManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
