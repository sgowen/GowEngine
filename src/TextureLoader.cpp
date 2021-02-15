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
    AssetHandler* ah = AssetHandlerFactory::create();
    
    const FileData textureData = ah->loadAsset(t._descriptor._filePath.c_str());
    
    if (IS_IOS)
    {
        stbi_convert_iphone_png_to_rgb(1);
    }
    
    t._data = stbi_load_from_memory(textureData._data, (int)textureData._length, &t._width, &t._height, &t._numChannels, 0);
    assert(t._data != NULL);

    ah->releaseAsset(textureData);

    AssetHandlerFactory::destroy(ah);
}

void TextureLoader::releaseTextureData(Texture& t)
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
