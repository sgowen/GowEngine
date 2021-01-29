//
//  Font.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Font.hpp"

#include "SpriteBatcher.hpp"
#include "TextureRegion.hpp"

#include <assert.h>

Font::Font(int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight)
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

Font::~Font()
{
    // Empty
}

void Font::renderText(SpriteBatcher& sb, std::string& text, float x, float y, float glyphWidth, float glyphHeight, int justification)
{
    assert(justification >= FONT_ALIGN_LEFT && justification <= FONT_ALIGN_RIGHT);
    
    unsigned long len = text.length();
    
	if (justification == FONT_ALIGN_CENTER)
	{
        float result = glyphWidth / 2;
		x -= len * result;
		x += glyphWidth / 2;
	}
	else if (justification == FONT_ALIGN_RIGHT)
	{
		x -= (text.length() - 1) * glyphWidth;
	}

	for (unsigned int i = 0; i < len; ++i)
	{
		int c = ((int)text.at(i));

        renderAsciiChar(sb, c, x, y, glyphWidth, glyphHeight);

		x += glyphWidth;
	}
}

void Font::renderAsciiChar(SpriteBatcher& sb, int asciiChar, float x, float y, float glyphWidth, float glyphHeight)
{
    assert(asciiChar >= 0 && asciiChar <= 175);
    
    sb.addSprite(_glyphs[asciiChar], x, y, glyphWidth, glyphHeight);
}
