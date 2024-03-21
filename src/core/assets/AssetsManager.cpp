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
    if (_assets.find(key) != _assets.end())
    {
        return;
    }
    
    _assets.emplace(std::piecewise_construct, std::make_tuple(key), std::make_tuple());
    _renderers.emplace(std::piecewise_construct, std::make_tuple(key), std::make_tuple());
}

void AssetsManager::deregisterAssets(std::string key)
{
    {
        auto q = _assets.find(key);
        assert(q != _assets.end());
        _assets.erase(q);
    }
    
    {
        auto q = _renderers.find(key);
        assert(q != _renderers.end());
        _renderers.erase(q);
    }
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
        Renderer& r = renderer(pair.first);
        
        if (a._isLoadedIntoEngine)
        {
            continue;
        }
        
        if (a._isDataLoaded)
        {
            _scriptMgr.loadIntoLuaAndFreeData(a._scripts);
            _shaderMgr.loadIntoOpenGLAndFreeData(a._shaders);
            _soundMgr.loadIntoOpenALAndFreeData(a._sounds);
            r.createDeviceDependentResources();
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
    
    loadData(true);
    loadData(false);
}

void AssetsManager::destroyDeviceDependentResources()
{
    _entityDefs.clear();
    _entityInputMappings.clear();
    _entityLayouts.clear();
    
    for (auto& pair : _renderers)
    {
        Renderer& r = pair.second;
        r.destroyDeviceDependentResources();
    }
    
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
    auto q = _renderers.find(name);
    assert(q != _renderers.end());
    
    return q->second;
}

bool AssetsManager::isRendererLoaded(std::string name)
{
    auto q = _renderers.find(name);
    
    return q != _renderers.end() && q->second.isLoadedIntoOpenGL();
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
    
    return _assets.size() > 0;
}

void AssetsManager::loadData(bool loadOnlyGlobalAssets)
{
    for (auto& pair : _assets)
    {
        std::string assetsFilePath = pair.first;
        Assets& a = pair.second;
        
        if (a._isLoadedIntoEngine)
        {
            continue;
        }
        
        bool isGlobalAsset = pair.first == FILE_PATH_ENGINE_ASSETS;
        
        if (isGlobalAsset != loadOnlyGlobalAssets)
        {
            continue;
        }
        
        a._isLoadedIntoEngine = false;
        a._isDataLoaded = false;
        
        if (a.needsToLoadDescriptors())
        {
            AssetsLoader::initWithJSONFile(a, assetsFilePath);
        }
        
        if (a._entityDefs != nullptr)
        {
            std::string filePath = a._entityDefs->_filePath;
            EntityDefsLoader::initWithJSONFile(_entityDefs, filePath);
        }
        
        for (auto& pair : a._entityInputMappings)
        {
            FileDescriptor& fd = pair.second;
            
            _entityInputMappings.emplace(std::piecewise_construct, std::make_tuple(pair.first), std::make_tuple(pair.first, fd._filePath));
            
            auto q = _entityInputMappings.find(pair.first);
            assert(q != _entityInputMappings.end());
            EntityInputMapping& eim = q->second;
            
            EntityInputMappingsLoader::loadEntityInputMapping(eim);
        }
        
        for (auto& pair : a._entityLayouts)
        {
            FileDescriptor& fd = pair.second;
            
            _entityLayouts.emplace(std::piecewise_construct, std::make_tuple(pair.first), std::make_tuple(pair.first, fd._name, fd._filePath));
        }
        
        if (a._renderer != nullptr)
        {
            auto q = _renderers.find(assetsFilePath);
            assert(q != _renderers.end());
            Renderer& r = q->second;
            RendererLoader::initWithJSONFile(r, a._renderer->_filePath);
        }
        
        _scriptMgr.loadData(a._scripts);
        _shaderMgr.loadData(a._shaders);
        _soundMgr.loadData(a._sounds);
        _textureMgr.loadData(a._textures);
        
        a._isDataLoaded = true;
    }
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
