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
	FontBatcher(Renderer& r, int maxBatchSize, std::string matrixName, std::string textureName, int glyphsPerRow, int glyphWidth, int glyphHeight);
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void begin();
    void addText(TextView& tv);
	void addText(std::string text, TextAlignment alignment, float xWeight, float yWeight, float glyphWidthWeight);
    void end(Shader& s);

private:
    Renderer& _renderer;
    SpriteBatcher _spriteBatcher;
    std::string _matrixName;
    std::string _textureName;
    std::vector<TextureRegion> _glyphs;
    int _glyphsPerRow;
    int _glyphWidth;
    int _glyphHeight;
    float _glyphWidthToHeightRatio;
};
