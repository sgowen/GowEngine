//
//  TextureManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "TextureManager.hpp"

#include "TextureDescriptor.hpp"
#include "OpenGLWrapper.hpp"

void TextureManager::loadTextures(std::vector<TextureDescriptor>& tds)
{
    assert(_textures.size() == 0);
    
    for (auto& td : tds)
    {
        _textures.emplace(td._name, Texture{td});
    }
    
    for (std::map<std::string, Texture>::iterator i = _textures.begin(); i != _textures.end(); ++i)
    {
        Texture& t = i->second;
        
        _loader.loadTexture(t);
        OGL.loadTexture(t);
        _loader.unloadTexture(t);
    }
}

void TextureManager::unloadTextures(std::vector<TextureDescriptor>& tds)
{
    for (std::map<std::string, Texture>::iterator i = _textures.begin(); i != _textures.end(); ++i)
    {
        Texture& t = i->second;
        OGL.unloadTexture(t);
    }
    _textures.clear();
}

Texture& TextureManager::texture(std::string name)
{
    auto q = _textures.find(name);
    assert(q != _textures.end());
    return q->second;
}

std::map<std::string, Texture>& TextureManager::textures()
{
    return _textures;
}
