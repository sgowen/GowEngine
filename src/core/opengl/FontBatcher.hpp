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
struct Matrix;

class FontBatcher
{
public:
	FontBatcher(std::string font, uint32_t maxBatchSize);
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addText(Matrix& m, TextView& tv);
	void addText(Matrix& m, std::string text, uint8_t alignment, float xWeight, float yWeight, float glyphWidthWeight);
    void end(Matrix& m, Shader& s, Color& c = Color::ONE);

private:
    std::vector<TextureRegion> _glyphs;
    std::string _font;
    SpriteBatcher _spriteBatcher;
};
