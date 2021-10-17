//
//  FontBatcher.hpp
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
#include "ShaderInput.hpp"

class Renderer;
struct Shader;
struct Texture;
struct TextureRegion;

class FontBatcher
{
public:
	FontBatcher(std::string font, std::string matrix, uint32_t maxBatchSize);
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addText(Renderer& r, TextView& tv);
	void addText(Renderer& r, std::string text, uint8_t alignment, float xWeight, float yWeight, float glyphWidthWeight);
    void end(Renderer& r, Shader& s);

private:
    std::vector<TextureRegion> _glyphs;
    std::string _font;
    std::string _matrix;
    SpriteBatcher _spriteBatcher;
};
