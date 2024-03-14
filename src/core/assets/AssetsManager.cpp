//
//  AssetsManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
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
            _shaderMgr.loadIntoOpenGLAndFreeData(a._shaderDescriptors);
            _soundMgr.loadIntoOpenALAndFreeData(a._soundDescriptors);
            _textureMgr.loadIntoOpenGLAndFreeData(a._textureDescriptors);
            
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
        
        _scriptMgr.loadData(a._scriptDescriptors);
        _shaderMgr.loadData(a._shaderDescriptors);
        _soundMgr.loadData(a._soundDescriptors);
        _textureMgr.loadData(a._textureDescriptors);
        
        a._isDataLoaded = true;
    }
}

void AssetsManager::destroyDeviceDependentResources()
{
    _scriptMgr.reset();
    _shaderMgr.reset();
    _soundMgr.reset();
    _textureMgr.reset();
    
    for (auto& pair : _assets)
    {
        Assets& a = pair.second;
        
        a._isLoadedIntoEngine = false;
        a._isDataLoaded = false;
    }
}

Entity* AssetsManager::createEntity(EntityInstanceDef eid)
{
    EntityDef& ed = getEntityDef(eid._key);
    return createEntity(ed, eid);
}

Entity* AssetsManager::createEntity(EntityDef& ed, EntityInstanceDef eid)
{
    return new Entity(ed, eid);
}

EntityDef& AssetsManager::getEntityDef(uint32_t fourCCName)
{
    auto q = _entityDefs.find(fourCCName);
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
_scriptMgr(),
_shaderMgr(),
_soundMgr(),
_textureMgr(),
_stateTime(0)
{
    // Empty
}
