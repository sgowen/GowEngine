//
//  FontBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "FontBatcher.hpp"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureRegion.hpp"
#include "AssetManager.hpp"
#include "KeyboardEvent.hpp"

#include <assert.h>

FontBatcher::FontBatcher(Renderer& r, int maxBatchSize, std::string matrixName, std::string textureName, int glyphsPerRow, int glyphWidth, int glyphHeight) :
_renderer(r),
_spriteBatcher(maxBatchSize),
_matrixName(matrixName),
_textureName(textureName),
_glyphsPerRow(glyphsPerRow),
_glyphWidth(glyphWidth),
_glyphHeight(glyphHeight),
_glyphWidthToHeightRatio(_glyphHeight / (float)_glyphWidth)
{
    // Empty
}

void FontBatcher::createDeviceDependentResources()
{
    _spriteBatcher.createDeviceDependentResources();
    
    Texture& t = ASSETS.texture(_textureName);
    int x = 0;
    int y = 0;
    for (int i = 0; i < 176; ++i)
    {
        _glyphs.emplace_back(x, y, _glyphWidth, _glyphHeight, t._width, t._height);

        x += _glyphWidth;

        if (x == _glyphsPerRow * _glyphWidth)
        {
            x = 0;
            y += _glyphHeight;
        }
    }
}

void FontBatcher::releaseDeviceDependentResources()
{
    _spriteBatcher.releaseDeviceDependentResources();
    
    _glyphs.clear();
}

void FontBatcher::begin()
{
    _spriteBatcher.begin();
}

void FontBatcher::addText(TextView& tv)
{
    if (tv._visibility == TEXV_HIDDEN)
    {
        return;
    }
    
    size_t len = tv._text.length();
    
    Matrix& m = _renderer.matrix(_matrixName);
    float matrixWidth = m._desc.width();
    float matrixHeight = m._desc.height();
    
    float x = matrixWidth * tv._xWeight;
    float y = matrixHeight * tv._yWeight;
    float glyphWidth = matrixWidth * tv._glyphWidthWeight;
    float glyphHeight = glyphWidth * _glyphWidthToHeightRatio;
    
    if (tv._alignment == TEXA_CENTER)
    {
        float result = glyphWidth / 2;
        x -= len * result;
        x += glyphWidth / 2;
    }
    else if (tv._alignment == TEXA_RIGHT)
    {
        x -= (len - 1) * glyphWidth;
    }

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t c = ((uint8_t)tv._text.at(i));
        
        if (c == GOW_KEY_NEW_LINE)
        {
            y += glyphHeight;
            continue;
        }

        assert(c >= 0 && c <= 175);
        _spriteBatcher.addSprite(_glyphs[c], x, y, glyphWidth, glyphHeight);

        x += glyphWidth;
    }
}

void FontBatcher::addText(std::string text, TextAlignment alignment, float xWeight, float yWeight, float glyphWidthWeight)
{
    TextView tv(text, alignment, xWeight, yWeight, glyphWidthWeight);
    addText(tv);
}

void FontBatcher::end(Shader& s)
{
    Matrix& m = _renderer.matrix(_matrixName);
    Texture& t = ASSETS.texture(_textureName);
    _spriteBatcher.end(s, m._matrix, t);
}
