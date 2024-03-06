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

#define FILE_PATH_ENGINE_ASSETS "engine/json/Assets.json"
#define FILE_PATH_ENGINE_CONFIG "engine/json/Config.json"
#define FILE_PATH_ENGINE_RENDERER "engine/json/Renderer.json"
#define FILE_PATH_SHADER_FRAGMENT_FRAMEBUFFER "engine/shaders/framebuffer.fsh"
#define FILE_PATH_SHADER_VERTEX_FRAMEBUFFER "engine/shaders/framebuffer.vsh"
#define FILE_PATH_SHADER_FRAGMENT_GEOMETRY "engine/shaders/geometry.fsh"
#define FILE_PATH_SHADER_VERTEX_GEOMETRY "engine/shaders/geometry.vsh"
#define FILE_PATH_SHADER_FRAGMENT_LIGHTS "engine/shaders/lights.fsh"
#define FILE_PATH_SHADER_VERTEX_LIGHTS "engine/shaders/lights.vsh"
#define FILE_PATH_SHADER_FRAGMENT_SHOCKWAVE "engine/shaders/shockwave.fsh"
#define FILE_PATH_SHADER_VERTEX_SHOCKWAVE "engine/shaders/shockwave.vsh"
#define FILE_PATH_SHADER_FRAGMENT_SPRITE "engine/shaders/sprite.fsh"
#define FILE_PATH_SHADER_VERTEX_SPRITE "engine/shaders/sprite.vsh"
#define FILE_PATH_TEXTURE_FONT "engine/textures/sf_font.png"

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
    if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_ASSETS))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_CONFIG))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_ENGINE_RENDERER))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_FRAMEBUFFER))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_FRAMEBUFFER))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_GEOMETRY))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_GEOMETRY))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_LIGHTS))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_LIGHTS))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SHOCKWAVE))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SHOCKWAVE))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_FRAGMENT_SPRITE))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_SHADER_VERTEX_SPRITE))
    {
        return true;
    }
    else if (StringUtil::doesStringEndWithString(filePath, FILE_PATH_TEXTURE_FONT))
    {
        return true;
    }
    
    return false;
}
