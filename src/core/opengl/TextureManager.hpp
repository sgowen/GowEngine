//
//  TextureManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "deps/stb/TextureLoader.hpp"
#include "Texture.hpp"

#include <map>
#include <string>

class TextureManager
{
public:
    void loadTextures(std::vector<TextureDescriptor>& tds);
    void loadTexture(Texture& t);
    void unloadTextures(std::vector<TextureDescriptor>& tds);
    Texture& texture(std::string name);
    bool isTextureLoaded(std::string name);
    bool isTextureLoaded(Texture& texture);
    std::map<std::string, Texture>& textures();
    
private:
    TextureLoader _loader;
    std::map<std::string, Texture> _textures;
};
