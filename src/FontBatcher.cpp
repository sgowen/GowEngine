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

#include <assert.h>

FontBatcher::FontBatcher(int maxBatchSize, int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight) :
_spriteBatcher(maxBatchSize),
_glyphWidthToHeightRatio(glyphHeight / (float)glyphWidth),
_matrixWidth(1),
_matrixHeight(1)
{
	int x = offsetX;
	int y = offsetY;

	for (int i = 0; i < 176; ++i)
	{
		_glyphs.emplace_back(x, y, glyphWidth, glyphHeight, textureWidth, textureHeight);

		x += glyphWidth;

		if (x == offsetX + glyphsPerRow * glyphWidth)
		{
			x = offsetX;
			y += glyphHeight;
		}
	}
}

void FontBatcher::createDeviceDependentResources()
{
    _spriteBatcher.createDeviceDependentResources();
}

void FontBatcher::releaseDeviceDependentResources()
{
    _spriteBatcher.releaseDeviceDependentResources();
}

void FontBatcher::setMatrixSize(float matrixWidth, float matrixHeight)
{
    mat4_identity(_matrix);
    mat4_ortho(_matrix, 0, matrixWidth, 0, matrixHeight, -1, 1);
    _matrixWidth = matrixWidth;
    _matrixHeight = matrixHeight;
}

void FontBatcher::configure(TextView& tv, float xWeight, float yWeight, float glyphWidthWeight)
{
    tv._x = _matrixWidth * xWeight;
    tv._y = _matrixHeight * yWeight;
    tv._glyphWidth = _matrixWidth * glyphWidthWeight;
    tv._glyphHeight = tv._glyphWidth * _glyphWidthToHeightRatio;
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
    
    float x = tv._x;
    
    if (tv._alignment == TEXA_CENTER)
    {
        float result = tv._glyphWidth / 2;
        x -= len * result;
        x += tv._glyphWidth / 2;
    }
    else if (tv._alignment == TEXA_RIGHT)
    {
        x -= (len - 1) * tv._glyphWidth;
    }

    for (size_t i = 0; i < len; ++i)
    {
        uint8_t c = ((uint8_t)tv._text.at(i));

        assert(c >= 0 && c <= 175);
        _spriteBatcher.addSprite(_glyphs[c], x, tv._y, tv._glyphWidth, tv._glyphHeight);

        x += tv._glyphWidth;
    }
}

void FontBatcher::addText(TextAlignment alignment, std::string text, float x, float y, float glyphWidth, float glyphHeight)
{
    TextView tv(alignment, text, TEXV_VISIBLE, x, y, glyphWidth, glyphHeight);
    addText(tv);
}

void FontBatcher::end(Shader& s, Texture& t)
{
    _spriteBatcher.end(s, _matrix, t);
}
