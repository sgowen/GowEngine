//
//  AssetsManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

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
    ++_stateTime;
    
    if (_isLoadingData)
    {
        return;
    }
    
    if (_isLoadedIntoEngine)
    {
        return;
    }

    std::map<std::string, Shader>& shaders = _shaderMgr.shaders();
    if (shaders.empty())
    {
        return;
    }
    for (auto& pair : shaders)
    {
        Shader& s = pair.second;
        if (s._vertexShaderFileData == nullptr ||
            s._fragmentShaderFileData == nullptr)
        {
            return;
        }
    }
    for (auto& pair : shaders)
    {
        Shader& s = pair.second;
        _shaderMgr.loadShaderIntoOpenGL(s);
    }

    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _textureMgr.loadTextures(a._textureDescriptors);
    }
    
    THREAD_MGR.tearDownThreadIfRunning("AssetsManager");
    _isLoadedIntoEngine = true;
}

void thread_createDeviceDependentResources(void* arg)
{
    assert(arg != nullptr);

    AssetsManager* am = static_cast<AssetsManager*>(arg);
    am->createDeviceDependentResources();
}

void AssetsManager::createDeviceDependentResourcesAsync()
{
    if (THREAD_MGR.isThreadRunning("AssetsManager"))
    {
        return;
    }

    _stateTime = 0;
    _isLoadingData = true;
    _isLoadedIntoEngine = false;
    
    THREAD_MGR.spawnThread("AssetsManager", thread_createDeviceDependentResources, this);
}

void AssetsManager::createDeviceDependentResources()
{
    _stateTime = 0;
    _isLoadingData = true;
    _isLoadedIntoEngine = false;
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.loadShaders(a._shaderDescriptors);
        _soundMgr.loadSounds(a._soundDescriptors);
//        _textureMgr.loadTextures(a._textureDescriptors);
    }
    
    _isLoadingData = false;
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

OpenALSoundWrapper* AssetsManager::sound(std::string soundID)
{
    return _soundMgr.sound(soundID);
}

std::map<std::string, OpenALSoundWrapper*>& AssetsManager::sounds()
{
    return _soundMgr.sounds();
}

OpenALSoundWrapper* AssetsManager::music()
{
    return _soundMgr.music();
}

Font& AssetsManager::font(std::string name)
{
    Font* ret = nullptr;
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        Font* f = a.font(name);
        if (f != nullptr)
        {
            ret = f;
            break;
        }
    }
    
    assert(ret != nullptr);
    
    return *ret;
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
    TextureRegion* ret = nullptr;

    for (auto& pair : _assets)
    {
        ret = pair.second.textureRegion(key, stateTime);
        if (ret != nullptr)
        {
            break;
        }
    }

    assert(ret != nullptr);

    return *ret;
}

uint32_t AssetsManager::getStateTime()
{
    return _stateTime;
}

bool AssetsManager::isLoaded()
{
    return _isLoadedIntoEngine;
}

AssetsManager::AssetsManager() :
_shaderMgr(),
_soundMgr(),
_textureMgr(),
_stateTime(0),
_isLoadingData(false),
_isLoadedIntoEngine(false)
{
    // Empty
}
