//
//  FontRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "SpriteBatcher.hpp"
#include "ShaderInput.hpp"
#include "TextView.hpp"

struct Shader;
struct Texture;
struct TextureRegion;

class FontRenderer
{
public:
	FontRenderer(int maxBatchSize, int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight);
    ~FontRenderer() {}
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    
    void renderText(Shader& s, mat4& matrix, Texture& t, TextView& tv);
	void renderText(Shader& s, mat4& matrix, Texture& t, std::string text, float x, float y, float glyphWidth, float glyphHeight, TextAlignment textAlignment = TextAlignment_LEFT);
    void renderAsciiChar(int asciiChar, float x, float y, float glyphWidth, float glyphHeight);

private:
    SpriteBatcher _spriteBatcher;
    std::vector<TextureRegion> _glyphs;
};
