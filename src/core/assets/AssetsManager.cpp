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
    if (_isLoaded)
    {
        return;
    }

    std::map<std::string, Shader>& shaders = _shaderMgr.shaders();
    for (auto& pair : shaders)
    {
        Shader& s = pair.second;
        if (s._program == 0 &&
            s._vertexShaderFileData != nullptr &&
            s._fragmentShaderFileData != nullptr)
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
            t._data != nullptr)
        {
            _textureMgr.loadTexture(t);
            return;
        }
    }

    if (areAssetsLoaded())
    {
        THREAD_MGR.tearDownThreadIfRunning("AssetsManager");
        _isLoaded = true;
    }
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

    _isLoaded = false;
    THREAD_MGR.spawnThread("AssetsManager", thread_createDeviceDependentResources, this);
}

void AssetsManager::createDeviceDependentResources()
{
    _isLoaded = false;
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

SoundWrapper* AssetsManager::sound(std::string soundID)
{
    return _soundMgr.sound(soundID);
}

std::map<std::string, SoundWrapper*>& AssetsManager::sounds()
{
    return _soundMgr.sounds();
}

SoundWrapper* AssetsManager::music()
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

bool AssetsManager::isLoaded()
{
    return _isLoaded;
}

bool AssetsManager::areAssetsLoaded()
{
    size_t numShaders = 0;
    size_t numSounds = 0;
    size_t numTextures = 0;
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

    std::map<std::string, SoundWrapper*>& sounds = _soundMgr.sounds();
    size_t expectedNumSounds = sounds.size();
    if (_soundMgr.music() != nullptr)
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
_textureMgr(),
_isLoaded(false)
{
    // Empty
}
