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
	FontBatcher(uint32_t maxBatchSize, std::string matrixName, std::string textureName, uint8_t glyphsPerRow, uint8_t glyphWidth, uint8_t glyphHeight);
    
    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addText(Renderer& r, TextView& tv);
	void addText(Renderer& r, std::string text, TextAlignment alignment, float xWeight, float yWeight, float glyphWidthWeight);
    void end(Renderer& r, Shader& s);

private:
    SpriteBatcher _spriteBatcher;
    std::string _matrixName;
    std::string _textureName;
    std::vector<TextureRegion> _glyphs;
    uint8_t _glyphsPerRow;
    uint8_t _glyphWidth;
    uint8_t _glyphHeight;
    float _glyphWidthToHeightRatio;
};
