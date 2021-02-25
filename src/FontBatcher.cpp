//
//  FontBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "FontBatcher.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureRegion.hpp"
#include "AssetManager.hpp"

#include <assert.h>

FontBatcher::FontBatcher(int maxBatchSize, std::string textureName, int glyphsPerRow, int glyphWidth, int glyphHeight) :
_spriteBatcher(maxBatchSize),
_textureName(textureName),
_glyphsPerRow(glyphsPerRow),
_glyphWidth(glyphWidth),
_glyphHeight(glyphHeight),
_glyphWidthToHeightRatio(_glyphHeight / (float)_glyphWidth),
_matrixWidth(1),
_matrixHeight(1)
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

void FontBatcher::setMatrixSize(float matrixWidth, float matrixHeight)
{
    mat4_identity(_matrix);
    mat4_ortho(_matrix, 0, matrixWidth, 0, matrixHeight, -1, 1);
    _matrixWidth = matrixWidth;
    _matrixHeight = matrixHeight;
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
    
    float x = _matrixWidth * tv._xWeight;
    float y = _matrixHeight * tv._yWeight;
    float glyphWidth = _matrixWidth * tv._glyphWidthWeight;
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
    Texture& t = ASSETS.texture(_textureName);
    _spriteBatcher.end(s, _matrix, t);
}
