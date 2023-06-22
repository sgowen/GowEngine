//
//  TextureManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void TextureManager::loadTextures(std::vector<TextureDescriptor>& tds)
{
    for (auto& td : tds)
    {
        _textures.emplace(td._name, Texture{td});
        Texture& t = texture(td._name);
        _loader.loadTexture(t);
    }
}

void TextureManager::loadTextureIntoOpenGL(Texture& t)
{
    LOG("loadTextureIntoOpenGL %s", t._desc._filePath.c_str());
    
    OGL.loadTexture(t);
    _loader.unloadTexture(t);
}

void TextureManager::unloadTextures(std::vector<TextureDescriptor>& tds)
{
    for (std::map<std::string, Texture>::iterator i = _textures.begin(); i != _textures.end(); ++i)
    {
        Texture& t = i->second;
        if (isTextureLoaded(t))
        {
            OGL.unloadTexture(t);
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
        return isTextureLoaded(q->second);
    }
    
    return false;
}

bool TextureManager::isTextureLoaded(Texture& texture)
{
    return texture._texture != 0;
}

std::map<std::string, Texture>& TextureManager::textures()
{
    return _textures;
}
