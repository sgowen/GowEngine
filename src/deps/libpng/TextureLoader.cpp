//
//  TextureLoader.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

extern "C"
{
    #include "png.h"
}

struct PngImageData
{
    int width;
    int height;
    int size;
    GLenum gl_color_format;
    void* data;
};

struct DataHandle
{
    png_byte* data;
    png_size_t size;
};

struct ReadDataHandle
{
    DataHandle data;
    png_size_t offset;
};

struct PngInfo
{
    png_uint_32 width;
    png_uint_32 height;
    int color_type;
};

static void readPngDataCallback(png_structp png_ptr, png_byte* raw_data, png_size_t read_length)
{
    ReadDataHandle* handle = (ReadDataHandle*) png_get_io_ptr(png_ptr);
    const png_byte* png_src = handle->data.data + handle->offset;
    
    memcpy(raw_data, png_src, read_length);
    handle->offset += read_length;
}

static PngInfo readAndUpdateInfo(const png_structp png_ptr, const png_infop info_ptr)
{
    png_uint_32 width, height;
    int bit_depth, color_type;
    
    png_read_info(png_ptr, info_ptr);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, nullptr, nullptr, nullptr);
    
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
    }
    
    if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
    {
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }
    
    if (bit_depth < 8)
    {
        png_set_packing(png_ptr);
    }
    else if (bit_depth == 16)
    {
        png_set_scale_16(png_ptr);
    }
    
    png_read_update_info(png_ptr, info_ptr);
    
    color_type = png_get_color_type(png_ptr, info_ptr);
    
    PngInfo ret;
    ret.width = width;
    ret.height = height;
    ret.color_type = color_type;
    
    return ret;
}

static DataHandle readEntirePngImage(const png_structp png_ptr, const png_infop info_ptr, const png_uint_32 height)
{
    const png_size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    const int data_length = (int) row_size * height;
    assert(row_size > 0);
    
    png_byte* raw_image = (png_byte*) malloc(data_length);
    assert(raw_image != nullptr);
    
    std::vector <png_byte*> row_ptrs;
    row_ptrs.resize(height);
    
    png_uint_32 i;
    for (i = 0; i < height; ++i)
    {
        row_ptrs[i] = raw_image + i * row_size;
    }
    
    png_read_image(png_ptr, &row_ptrs[0]);
    
    DataHandle ret;
    ret.data = raw_image;
    ret.size = static_cast<png_size_t>(data_length);
    
    return ret;
}

static GLenum getGlColorFormat(const int png_color_format)
{
    assert(png_color_format == PNG_COLOR_TYPE_GRAY || png_color_format == PNG_COLOR_TYPE_RGB_ALPHA || png_color_format == PNG_COLOR_TYPE_GRAY_ALPHA);
    
    switch (png_color_format)
    {
        case PNG_COLOR_TYPE_GRAY:
            return GL_LUMINANCE;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            return GL_RGBA;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            return GL_LUMINANCE_ALPHA;
    }
    
    return 0;
}

static PngImageData getPngImageDataFromFileData(const void* png_data, const int png_data_size)
{
    assert(png_data != nullptr && png_data_size > 8);
    assert(png_check_sig((png_const_bytep)png_data, 8));
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    assert(png_ptr != nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr != nullptr);
    
    DataHandle data_handle;
    data_handle.data = (png_byte*) png_data;
    data_handle.size = static_cast<png_size_t>(png_data_size);
    
    ReadDataHandle png_data_handle;
    png_data_handle.data = data_handle;
    png_data_handle.offset = 0;
    
    png_set_read_fn(png_ptr, &png_data_handle, readPngDataCallback);
    
    assert(setjmp(png_jmpbuf(png_ptr)) == 0);
    
    const PngInfo png_info = readAndUpdateInfo(png_ptr, info_ptr);
    const DataHandle raw_image = readEntirePngImage(png_ptr, info_ptr, png_info.height);
    
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    PngImageData ret;
    ret.width = static_cast<int>(png_info.width);
    ret.height = static_cast<int>(png_info.height);
    ret.size = (int) raw_image.size;
    ret.gl_color_format = getGlColorFormat(png_info.color_type);
    ret.data = raw_image.data;
    
    return ret;
}

void TextureLoader::loadTexture(Texture& t)
{
    const FileData fd = ASSET_HANDLER.loadAsset(t._desc._filePath);
    
    const PngImageData pngImageData = getPngImageDataFromFileData(fd._data, (int)fd._length);
    
    t._data = (uint8_t*)pngImageData.data;
    t._width = pngImageData.width;
    t._height = pngImageData.height;
    t._format = pngImageData.gl_color_format;
    
    assert(t._data != nullptr);
    
    ASSET_HANDLER.unloadAsset(fd);
}

void TextureLoader::unloadTexture(Texture& t)
{
    free((void*)t._data);
    t._data = nullptr;
}
