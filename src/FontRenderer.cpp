//
//  FontRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "FontRenderer.hpp"

#include "Shader.hpp"
#include "Texture.hpp"
#include "TextureRegion.hpp"

#include <assert.h>

FontRenderer::FontRenderer(int maxBatchSize, int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight) :
_spriteBatcher(maxBatchSize)
{
	int x = offsetX;
	int y = offsetY;

	for (int i = 0; i < 176; ++i)
	{
		_glyphs.emplace_back(x, y, glyphWidth, glyphHeight, textureWidth, textureHeight,0);

		x += glyphWidth;

		if (x == offsetX + glyphsPerRow * glyphWidth)
		{
			x = offsetX;
			y += glyphHeight;
		}
	}
}

FontRenderer::~FontRenderer()
{
    // Empty
}

void FontRenderer::createDeviceDependentResources()
{
    _spriteBatcher.createDeviceDependentResources();
}

void FontRenderer::releaseDeviceDependentResources()
{
    _spriteBatcher.releaseDeviceDependentResources();
}

void FontRenderer::renderText(Shader& s, mat4& matrix, Texture& t, TextView& tv)
{
    assert(tv._textAlignment >= TextAlignment_LEFT && tv._textAlignment <= TextAlignment_RIGHT);
    
    unsigned long len = tv._text.length();
    
    float x = tv._x;
    
    if (tv._textAlignment == TextAlignment_CENTER)
    {
        float result = tv._glyphWidth / 2;
        x -= len * result;
        x += tv._glyphWidth / 2;
    }
    else if (tv._textAlignment == TextAlignment_RIGHT)
    {
        x -= (len - 1) * tv._glyphWidth;
    }

    _spriteBatcher.begin();
    for (unsigned int i = 0; i < len; ++i)
    {
        int c = ((int)tv._text.at(i));

        renderAsciiChar(c, x, tv._y, tv._glyphWidth, tv._glyphHeight);

        x += tv._glyphWidth;
    }
    _spriteBatcher.end(s, matrix, t);
}

void FontRenderer::renderText(Shader& s, mat4& matrix, Texture& t, std::string text, float x, float y, float glyphWidth, float glyphHeight, TextAlignment textAlignment)
{
    TextView tv(text, x, y, glyphWidth, glyphHeight, textAlignment);
    renderText(s, matrix, t, tv);
}

void FontRenderer::renderAsciiChar(int asciiChar, float x, float y, float glyphWidth, float glyphHeight)
{
    assert(asciiChar >= 0 && asciiChar <= 175);
    
    _spriteBatcher.addSprite(_glyphs[asciiChar], x, y, glyphWidth, glyphHeight);
}
