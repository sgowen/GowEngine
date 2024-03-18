//
//  AssetsManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void AssetsManager::registerAssets(std::string key)
{
    assert(_assets.find(key) == _assets.end());
    _assets.emplace(key, Assets());
}

void AssetsManager::deregisterAssets(std::string key)
{
    auto q = _assets.find(key);
    assert(q != _assets.end());
    _assets.erase(q);
}

void AssetsManager::update()
{
    if (isLoaded())
    {
        return;
    }
    
    ++_stateTime;
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        if (a._isLoadedIntoEngine)
        {
            continue;
        }
        
        if (a._isDataLoaded)
        {
            _scriptMgr.loadIntoLuaAndFreeData(a._scripts);
            _shaderMgr.loadIntoOpenGLAndFreeData(a._shaders);
            _soundMgr.loadIntoOpenALAndFreeData(a._sounds);
            _textureMgr.loadIntoOpenGLAndFreeData(a._textures);
            
            a._isLoadedIntoEngine = true;
            a._isDataLoaded = false;
        }
    }
    
    if (isLoaded())
    {
        THREAD_MGR.tearDownThreadIfRunning("AssetsManager");
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
    assert(THREAD_MGR.isThreadRunning("AssetsManager") == false);
    
    THREAD_MGR.spawnThread("AssetsManager", thread_createDeviceDependentResources, this);
}

void AssetsManager::createDeviceDependentResources()
{
    _stateTime = 0;
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        
        if (a._isLoadedIntoEngine)
        {
            continue;
        }
        
        a._isLoadedIntoEngine = false;
        a._isDataLoaded = false;
        
        if (a.needsToLoadDescriptors())
        {
            std::string filePath = pair.first;
            AssetsLoader::initWithJSONFile(a, filePath);
        }
        
        if (a._entityDefs != nullptr)
        {
            std::string filePath = a._entityDefs->_filePath;
            EntityDefsLoader::initWithJSONFile(_entityDefs, filePath);
        }
        
        // TODO, a._entityInputMappings
        
        // TODO, a._entityLayouts
        
        if (a._renderer != nullptr)
        {
            std::string filePath = a._renderer->_filePath;
            RendererLoader::initWithJSONFile(_renderer, filePath);
        }
        
        _scriptMgr.loadData(a._scripts);
        _shaderMgr.loadData(a._shaders);
        _soundMgr.loadData(a._sounds);
        _textureMgr.loadData(a._textures);
        
        a._isDataLoaded = true;
    }
}

void AssetsManager::onWindowSizeChanged(uint16_t screenWidth, uint16_t screenHeight)
{
    _renderer.onWindowSizeChanged(screenWidth, screenHeight);
}

void AssetsManager::destroyDeviceDependentResources()
{
    _renderer.destroyDeviceDependentResources();
    
    _scriptMgr.reset();
    _shaderMgr.reset();
    _soundMgr.reset();
    _textureMgr.reset();
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        a.reset();
    }
}

EntityDef& AssetsManager::getEntityDef(uint32_t fourCCKey)
{
    auto q = _entityDefs.find(fourCCKey);
    assert(q != _entityDefs.end());
    
    return q->second;
}

EntityInputMapping& AssetsManager::entityInputMapping(std::string key)
{
    auto q = _entityInputMappings.find(key);
    assert(q != _entityInputMappings.end());

    return q->second;
}

EntityLayout& AssetsManager::entityLayout(uint32_t key)
{
    auto q = _entityLayouts.find(key);
    assert(q != _entityLayouts.end());

    return q->second;
}

uint32_t AssetsManager::getFirstLayout()
{
    return _entityLayouts.begin()->first;
}

Script& AssetsManager::script(std::string name)
{
    return _scriptMgr.script(name);
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

Renderer& AssetsManager::renderer(std::string name)
{
    // TODO
    return nullptr;
}

Texture& AssetsManager::texture(std::string name)
{
    return _textureMgr.texture(name);
}

bool AssetsManager::isTextureLoaded(std::string name)
{
    return _textureMgr.isTextureLoaded(name);
}

std::string AssetsManager::textureForRegionKey(std::string key)
{
    std::string ret = "";

    for (auto& pair : _assets)
    {
        ret = pair.second.textureForRegionKey(key);
        if (!ret.empty())
        {
            break;
        }
    }
    
    return ret;
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

Animation* AssetsManager::animation(std::string key)
{
    Animation* ret = nullptr;

    for (auto& pair : _assets)
    {
        ret = pair.second.animation(key);
        if (ret != nullptr)
        {
            break;
        }
    }

    return ret;
}

uint32_t AssetsManager::getStateTime()
{
    return _stateTime;
}

bool AssetsManager::isLoaded()
{
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        if (!a._isLoadedIntoEngine)
        {
            return false;
        }
    }
    
    return true;
}

AssetsManager::AssetsManager() :
_stateTime(0),
_scriptMgr(),
_shaderMgr(),
_soundMgr(),
_textureMgr()
{
    // Empty
}
