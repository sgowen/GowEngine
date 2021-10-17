//
//  FontBatcher.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

FontBatcher::FontBatcher(std::string font, std::string matrix, uint32_t maxBatchSize) :
_font(font),
_matrix(matrix),
_spriteBatcher(maxBatchSize)
{
    // Empty
}

void FontBatcher::createDeviceDependentResources()
{
    _spriteBatcher.createDeviceDependentResources();
}

void FontBatcher::destroyDeviceDependentResources()
{
    _spriteBatcher.destroyDeviceDependentResources();
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

    Font& f = ASSETS_MGR.font(_font);
    float glyphWidthToHeightRatio = f._glyphWidthToHeightRatio;
    
    if (_glyphs.empty())
    {
        Texture& t = ASSETS_MGR.texture(f._texture);
        int x = 0;
        int y = 0;
        for (int i = 0; i < 176; ++i)
        {
            _glyphs.emplace_back(x, y, f._glyphWidth, f._glyphHeight, t._width, t._height);

            x += f._glyphWidth;

            if (x == f._glyphsPerRow * f._glyphWidth)
            {
                x = 0;
                y += f._glyphHeight;
            }
        }
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

    Matrix& m = r.matrix(_matrix);
    float matrixWidth = m._desc.width();
    float matrixHeight = m._desc.height();
    float y = matrixHeight * tv._yWeight;

    int numCharsConsumed = 0;
    for (int rowLen : rows)
    {
        float x = matrixWidth * tv._xWeight;
        float glyphWidth = matrixWidth * tv._glyphWidthWeight;
        float glyphHeight = glyphWidth * glyphWidthToHeightRatio;

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

        for (int i = 0; i < rowLen; ++i)
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

void FontBatcher::addText(Renderer& r, std::string text, uint8_t alignment, float xWeight, float yWeight, float glyphWidthWeight)
{
    TextView tv(text, alignment, xWeight, yWeight, glyphWidthWeight);
    addText(r, tv);
}

void FontBatcher::end(Renderer& r, Shader& s)
{
    Matrix& m = r.matrix(_matrix);
    Font& f = ASSETS_MGR.font(_font);
    Texture& t = ASSETS_MGR.texture(f._texture);
    _spriteBatcher.end(s, m._matrix, t);
}
