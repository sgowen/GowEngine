//
//  FontBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "SpriteBatcher.hpp"
#include "ShaderInput.hpp"
#include "core/assets/TextView.hpp"
#include "ShaderInput.hpp"
#include "Color.hpp"

class Renderer;
struct Shader;
struct TextureRegion;
struct Color;

class FontBatcher
{
public:
	FontBatcher(std::string font, std::string matrix, uint32_t maxBatchSize);
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addText(Renderer& r, TextView& tv);
	void addText(Renderer& r, std::string text, uint8_t alignment, float xWeight, float yWeight, float glyphWidthWeight);
    void end(Renderer& r, Shader& s, Color& c = Color::ONE);

private:
    std::vector<TextureRegion> _glyphs;
    std::string _font;
    std::string _matrix;
    SpriteBatcher _spriteBatcher;
};
