//
//  TextureLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "TextureLoader.hpp"

#include "Texture.hpp"
#include "AssetHandlerFactory.hpp"
#include "AssetHandler.hpp"
#include "FileData.hpp"
#include "PlatformMacros.hpp"
#include "OpenGLWrapper.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void TextureLoader::loadTextureData(Texture& t)
{
    const FileData fd = ASSET_HANDLER.loadAsset(t._desc._filePath);
    
    if (IS_IOS)
    {
        stbi_convert_iphone_png_to_rgb(1);
    }
    
    t._data = stbi_load_from_memory(fd._data, (int)fd._length, &t._width, &t._height, &t._numChannels, 0);
    assert(t._data != NULL);

    ASSET_HANDLER.unloadAsset(fd);
}

void TextureLoader::unloadTextureData(Texture& t)
{
    stbi_image_free(t._data);
    t._data = NULL;
}

void TextureLoader::loadTexture(Texture& t)
{
    OGL.loadTexture(t);
}

void TextureLoader::unloadTexture(Texture& t)
{
    OGL.unloadTexture(t);
}
