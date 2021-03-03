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
#include "deps/tinythreadpp/ThreadManager.hpp"

#include <assert.h>

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

void AssetsManager::update()
{
    std::map<std::string, Shader>& shaders = _shaderMgr.shaders();
    for (auto& pair : shaders)
    {
        Shader& s = pair.second;
        if (s._program == 0 &&
            s._vertexShaderFileData != NULL &&
            s._fragmentShaderFileData != NULL)
        {
            _shaderMgr.loadShader(s);
            return;
        }
    }
    
    std::map<std::string, Texture>& textures = _textureMgr.textures();
    for (auto& pair : textures)
    {
        Texture& t = pair.second;
        if (t._texture == 0 &&
            t._data != NULL)
        {
            _textureMgr.loadTexture(t);
            return;
        }
    }
}

void load(void* arg)
{
    assert(arg != NULL);
    
    AssetsManager* am = static_cast<AssetsManager*>(arg);
    am->createDeviceDependentResources();
    THREAD_MGR.tearDownThread("AssetsManager");
}

void AssetsManager::createDeviceDependentResourcesAsync()
{
    THREAD_MGR.spawnThread("AssetsManager", load, this);
}

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

void AssetsManager::destroyDeviceDependentResources()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.unloadShaders(a._shaderDescriptors);
        _soundMgr.unloadSounds(a._soundDescriptors);
        _textureMgr.unloadTextures(a._textureDescriptors);
    }
}

Shader& AssetsManager::shader(std::string name)
{
    return _shaderMgr.shader(name);
}

bool AssetsManager::isShaderLoaded(std::string name)
{
    return _shaderMgr.isShaderLoaded(name);
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

bool AssetsManager::isTextureLoaded(std::string name)
{
    return _textureMgr.isTextureLoaded(name);
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

bool AssetsManager::isLoaded()
{
    if (THREAD_MGR.isThreadRunning("AssetsManager"))
    {
        return false;
    }
    
    int numShaders = 0;
    int numSounds = 0;
    int numTextures = 0;
    for (auto& pair : _assets)
    {
        numShaders += pair.second._shaderDescriptors.size();
        numSounds += pair.second._soundDescriptors.size();
        numTextures += pair.second._textureDescriptors.size();
    }
    
    std::map<std::string, Shader>& shaders = _shaderMgr.shaders();
    if (numShaders != shaders.size())
    {
        return false;
    }
    
    std::map<uint16_t, SoundWrapper*>& sounds = _soundMgr.sounds();
    size_t expectedNumSounds = sounds.size();
    if (_soundMgr.music() != NULL)
    {
        ++expectedNumSounds;
    }
    if (numSounds != expectedNumSounds)
    {
        return false;
    }
    
    std::map<std::string, Texture>& textures = _textureMgr.textures();
    if (numTextures != textures.size())
    {
        return false;
    }
    
    for (auto& pair : shaders)
    {
        if (pair.second._program == 0)
        {
            return false;
        }
    }
    
    for (auto& pair : textures)
    {
        if (pair.second._texture == 0)
        {
            return false;
        }
    }
    
    return true;
}

AssetsManager::AssetsManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
