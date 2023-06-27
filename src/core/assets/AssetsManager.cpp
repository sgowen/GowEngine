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
    if (_isLoadedIntoEngine)
    {
        return;
    }
    
    ++_stateTime;
    
    if (_isLoadingData)
    {
        return;
    }
    
    _shaderMgr.loadIntoOpenGLAndFreeData();
    _soundMgr.loadIntoOpenALAndFreeData();
    _textureMgr.loadIntoOpenGLAndFreeData();
    
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
    assert(THREAD_MGR.isThreadRunning("AssetsManager") == false);

    beginLoad();
    
    THREAD_MGR.spawnThread("AssetsManager", thread_createDeviceDependentResources, this);
}

void AssetsManager::createDeviceDependentResources()
{
    beginLoad();
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        _shaderMgr.prepare(a._shaderDescriptors);
        _soundMgr.prepare(a._soundDescriptors);
        _textureMgr.prepare(a._textureDescriptors);
    }
    
    _shaderMgr.loadData();
    _soundMgr.loadData();
    _textureMgr.loadData();
    
    _isLoadingData = false;
}

void AssetsManager::destroyDeviceDependentResources()
{
    _shaderMgr.reset();
    _soundMgr.reset();
    _textureMgr.reset();
}

Shader& AssetsManager::shader(std::string name)
{
    return _shaderMgr.shader(name);
}

bool AssetsManager::isShaderLoaded(std::string name)
{
    return _shaderMgr.isShaderLoaded(name);
}

Sound& AssetsManager::sound(std::string soundID)
{
    return _soundMgr.sound(soundID);
}

std::map<std::string, Sound>& AssetsManager::sounds()
{
    return _soundMgr.sounds();
}

bool AssetsManager::areSoundsLoaded()
{
    return _soundMgr.areSoundsLoaded();
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

bool AssetsManager::isFontLoaded(std::string name)
{
    Font& f = font(name);
    return _textureMgr.isTextureLoaded(f._texture);
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

void AssetsManager::beginLoad()
{
    _stateTime = 0;
    _isLoadingData = true;
    _isLoadedIntoEngine = false;
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
