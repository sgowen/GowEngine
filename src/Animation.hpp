//
//  Animation.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>

struct TextureRegion;

class Animation
{
public:
    Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int firstLoopingFrame, int xPadding, int yPadding, std::vector<uint16_t> frameTimes, int layer);
    ~Animation() {}

    TextureRegion& getTextureRegion(uint16_t stateTime);
    TextureRegion& getTextureRegionAtKeyFrame(uint16_t frameIndex);
    uint16_t getFrameIndex(uint16_t stateTime);

private:
    bool _isLooping;
    uint16_t _firstLoopingFrame;
    std::vector<uint16_t> _frameTimes;
    uint16_t _cycleTime;
    std::vector<TextureRegion> _textureRegions;
};
