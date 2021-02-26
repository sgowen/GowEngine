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

FontBatcher::FontBatcher(uint32_t maxBatchSize, std::string matrixName, std::string textureName, uint8_t glyphsPerRow, uint8_t glyphWidth, uint8_t glyphHeight) :
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

void FontBatcher::addText(Renderer& r, TextView& tv)
{
    if (tv._visibility == TEXV_HIDDEN)
    {
        return;
    }
    
    size_t len = tv._text.length();
    
    std::vector<int> rows;
    rows.push_back(0);
    int rowIndex = 0;
    for (size_t i = 0; i < len; ++i)
    {
        uint8_t c = ((uint8_t)tv._text.at(i));
        if (c == GOW_KEY_NEW_LINE)
        {
            rows.push_back(1);
            ++rowIndex;
            continue;
        }
        
        ++rows[rowIndex];
    }
    
    Matrix& m = r.matrix(_matrixName);
    float matrixWidth = m._desc.width();
    float matrixHeight = m._desc.height();
    float y = matrixHeight * tv._yWeight;
    
    int numCharsConsumed = 0;
    for (int rowLen : rows)
    {
        float x = matrixWidth * tv._xWeight;
        float glyphWidth = matrixWidth * tv._glyphWidthWeight;
        float glyphHeight = glyphWidth * _glyphWidthToHeightRatio;
        
        if (tv._alignment == TEXA_CENTER)
        {
            x -= rowLen * glyphWidth / 2;
            x += glyphWidth / 2;
        }
        else if (tv._alignment == TEXA_RIGHT)
        {
            x -= (rowLen - 1) * glyphWidth;
        }
        
        float startingXValue = x;

        for (size_t i = 0; i < rowLen; ++i)
        {
            uint8_t c = ((uint8_t)tv._text.at(i + numCharsConsumed));
            
            if (c == GOW_KEY_NEW_LINE)
            {
                x = startingXValue;
                y -= glyphHeight;
                continue;
            }

            assert(c >= 0 && c <= 175);
            _spriteBatcher.addSprite(_glyphs[c], x, y, glyphWidth, glyphHeight);

            x += glyphWidth;
        }
        numCharsConsumed += rowLen;
    }
}

void FontBatcher::addText(Renderer& r, std::string text, TextAlignment alignment, float xWeight, float yWeight, float glyphWidthWeight)
{
    TextView tv(text, alignment, xWeight, yWeight, glyphWidthWeight);
    addText(r, tv);
}

void FontBatcher::end(Renderer& r, Shader& s)
{
    Matrix& m = r.matrix(_matrixName);
    Texture& t = ASSETS.texture(_textureName);
    _spriteBatcher.end(s, m._matrix, t);
}
