//
//  TextureManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void TextureManager::prepare(std::vector<TextureDescriptor>& tds)
{
    for (auto& td : tds)
    {
        _textures.emplace(td._name, Texture{td});
    }
}

void TextureManager::loadData()
{
    for (auto& pair : _textures)
    {
        Texture& t = pair.second;
        _loader.loadData(t);
    }
}

void TextureManager::loadIntoOpenGLAndFreeData()
{
    for (auto& pair : _textures)
    {
        Texture& t = pair.second;
        OGL.loadTexture(t);
        _loader.freeData(t);
    }
}

void TextureManager::reset()
{
    for (auto& pair : _textures)
    {
        Texture& t = pair.second;
        if (t._glHandle > 0)
        {
            OGL.unloadTexture(t);
        }
        if (t._data != nullptr)
        {
            _loader.freeData(t);
        }
    }
    _textures.clear();
}

Texture& TextureManager::texture(std::string name)
{
    auto q = _textures.find(name);
    assert(q != _textures.end());
    return q->second;
}

bool TextureManager::isTextureLoaded(std::string name)
{
    auto q = _textures.find(name);
    if (q != _textures.end())
    {
        Texture& t = q->second;
        return t._glHandle > 0;
    }
    
    return false;
}
