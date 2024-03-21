//
//  EmbeddedAssetHandler.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/16/24.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#include <GowEngine/embeddedData/engine_assets_json.h>
#include <GowEngine/embeddedData/engine_config_json.h>
#include <GowEngine/embeddedData/engine_renderer_json.h>
#include <GowEngine/embeddedData/default_renderer_lua.h>
#include <GowEngine/embeddedData/framebuffer_fsh.h>
#include <GowEngine/embeddedData/framebuffer_vsh.h>
#include <GowEngine/embeddedData/geometry_fsh.h>
#include <GowEngine/embeddedData/geometry_vsh.h>
#include <GowEngine/embeddedData/lights_fsh.h>
#include <GowEngine/embeddedData/lights_vsh.h>
#include <GowEngine/embeddedData/shockwave_fsh.h>
#include <GowEngine/embeddedData/shockwave_vsh.h>
#include <GowEngine/embeddedData/sprite_fsh.h>
#include <GowEngine/embeddedData/sprite_vsh.h>
#include <GowEngine/embeddedData/sf_font_png.h>

FileData EmbeddedAssetHandler::loadAsset(std::string filePath)
{
    std::vector<uint8_t>* blob = new std::vector<uint8_t>();
    
    if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_ASSETS))
    {
        blob->insert(blob->begin(), std::begin(engine_assets_json), std::end(engine_assets_json));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_CONFIG))
    {
        blob->insert(blob->begin(), std::begin(engine_config_json), std::end(engine_config_json));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_RENDERER))
    {
        blob->insert(blob->begin(), std::begin(engine_renderer_json), std::end(engine_renderer_json));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SCRIPT_DEFAULT_RENDERER))
    {
        blob->insert(blob->begin(), std::begin(default_renderer_lua), std::end(default_renderer_lua));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_FRAMEBUFFER))
    {
        blob->insert(blob->begin(), std::begin(framebuffer_fsh), std::end(framebuffer_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_FRAMEBUFFER))
    {
        blob->insert(blob->begin(), std::begin(framebuffer_vsh), std::end(framebuffer_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_GEOMETRY))
    {
        blob->insert(blob->begin(), std::begin(geometry_fsh), std::end(geometry_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_GEOMETRY))
    {
        blob->insert(blob->begin(), std::begin(geometry_vsh), std::end(geometry_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_LIGHTS))
    {
        blob->insert(blob->begin(), std::begin(lights_fsh), std::end(lights_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_LIGHTS))
    {
        blob->insert(blob->begin(), std::begin(lights_vsh), std::end(lights_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SHOCKWAVE))
    {
        blob->insert(blob->begin(), std::begin(shockwave_fsh), std::end(shockwave_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SHOCKWAVE))
    {
        blob->insert(blob->begin(), std::begin(shockwave_vsh), std::end(shockwave_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SPRITE))
    {
        blob->insert(blob->begin(), std::begin(sprite_fsh), std::end(sprite_fsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SPRITE))
    {
        blob->insert(blob->begin(), std::begin(sprite_vsh), std::end(sprite_vsh));
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_TEXTURE_FONT))
    {
        blob->insert(blob->begin(), std::begin(sf_font_png), std::end(sf_font_png));
    }

    return FileData(static_cast<long>(blob->size()), blob->data(), (const void*)blob);
}

void EmbeddedAssetHandler::unloadAsset(const FileData& fileData)
{
    assert(fileData._fileHandle != nullptr);

    const std::vector<uint8_t>* handle = static_cast<const std::vector<uint8_t>*>(fileData._fileHandle);
    delete handle;
}

bool EmbeddedAssetHandler::isAssetEmbedded(std::string filePath)
{
    if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_ASSETS) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_CONFIG) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_RENDERER) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SCRIPT_DEFAULT_RENDERER) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_FRAMEBUFFER) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_FRAMEBUFFER) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_GEOMETRY) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_GEOMETRY) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_LIGHTS) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_LIGHTS) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SHOCKWAVE) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SHOCKWAVE) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SPRITE) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SPRITE) ||
        StringUtil::doesStringEndWithString(filePath, FILE_PATH_TEXTURE_FONT))
    {
        return true;
    }
    
    return false;
}
