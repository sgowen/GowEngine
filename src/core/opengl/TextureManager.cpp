//
//  TextureManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void TextureManager::loadData(std::map<std::string, TextureDescriptor>& textureDescriptors)
{
    for (auto& pair : textureDescriptors)
    {
        _textures.emplace(pair.first, Texture{pair.second});
        Texture& t = texture(pair.first);
        _loader.loadData(t);
    }
}

void TextureManager::loadIntoOpenGLAndFreeData(std::map<std::string, TextureDescriptor>& textureDescriptors)
{
    for (auto& pair : textureDescriptors)
    {
        Texture& t = texture(pair.first);
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
