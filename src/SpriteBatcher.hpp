//
//  SpriteBatcher.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "ShaderInput.hpp"
#include "Color.hpp"
#include "TextureRegion.hpp"

#include <stdint.h>
#include <vector>

struct Shader;
struct Texture;

class SpriteBatcher
{
public:
    SpriteBatcher(uint32_t maxBatchSize);

    void createDeviceDependentResources();
    void destroyDeviceDependentResources();
    void begin();
    void addSprite(TextureRegion& tr, float x, float y, float width, float height, float angle = 0, bool flipX = false);
    void end(Shader& s, mat4& matrix, Texture& t, const Color& colorFactor = Color::ONE);

private:
    uint32_t _maxBatchSize;
    std::vector<VERTEX_2D_TEXTURE> _vertices;
    uint32_t _vertexBuffer;
    uint32_t _indexBuffer;
};
