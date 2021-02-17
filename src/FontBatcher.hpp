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

struct Shader;
struct Texture;
struct TextureRegion;

class FontBatcher
{
public:
	FontBatcher(int maxBatchSize, int offsetX, int offsetY, int glyphsPerRow, int glyphWidth, int glyphHeight, int textureWidth, int textureHeight);
    ~FontBatcher() {}
    
    void createDeviceDependentResources();
    void releaseDeviceDependentResources();
    void setMatrixSize(float matrixWidth, float matrixHeight);
    void configure(TextView& tv, float xWeight, float yWeight, float glyphWidthWeight);
    void begin();
    void addText(TextView& tv);
	void addText(TextAlignment textAlignment, std::string text, float x, float y, float glyphWidth, float glyphHeight);
    void end(Shader& s, Texture& t);

private:
    SpriteBatcher _spriteBatcher;
    std::vector<TextureRegion> _glyphs;
    float _glyphWidthToHeightRatio;
    mat4 _matrix;
    float _matrixWidth;
    float _matrixHeight;
};
