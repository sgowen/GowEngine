//
//  Font.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define FONT_ALIGN_LEFT 0
#define FONT_ALIGN_CENTER 1
#define FONT_ALIGN_RIGHT 2

#include <string>
#include <vector>

class SpriteBatcher;
struct TextureRegion;

class Font
{
public:
	Font(int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight);
    ~Font();
    
	void renderText(SpriteBatcher& sb, std::string& text, float x, float y, float glyphWidth, float glyphHeight, int justification = FONT_ALIGN_LEFT);
    void renderAsciiChar(SpriteBatcher& sb, int asciiChar, float x, float y, float glyphWidth, float glyphHeight);

private:
    std::vector<TextureRegion> _glyphs;
};
