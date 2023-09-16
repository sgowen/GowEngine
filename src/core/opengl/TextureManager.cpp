//
//  TextureManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void TextureManager::loadData(std::vector<TextureDescriptor>& textureDescriptors)
{
    for (auto& td : textureDescriptors)
    {
        _textures.emplace(td._name, Texture{td});
        Texture& t = texture(td._name);
        _loader.loadData(t);
    }
}

void TextureManager::loadIntoOpenGLAndFreeData(std::vector<TextureDescriptor>& textureDescriptors)
{
    for (auto& td : textureDescriptors)
    {
        Texture& t = texture(td._name);
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
