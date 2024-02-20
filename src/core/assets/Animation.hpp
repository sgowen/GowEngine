//
//  Animation.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <vector>

struct TextureRegion;

class Animation
{
public:
    Animation(uint16_t x, uint16_t y, std::vector<uint16_t> regionWidths, std::vector<uint16_t> regionHeights, uint16_t animationWidth, uint16_t animationHeight, uint16_t textureWidth, uint16_t textureHeight, bool looping, uint16_t firstLoopingFrame, uint16_t xPadding, uint16_t yPadding, std::vector<uint16_t> frameTimes);

    TextureRegion& textureRegion(uint16_t stateTime);
    TextureRegion& textureRegionAtKeyFrame(uint16_t frameIndex);
    uint16_t frameIndex(uint16_t stateTime);
    uint16_t cycleTime();
    uint16_t cycleTimeBeforeFirstLoopingFrame();
    bool isLooping();

private:
    bool _isLooping;
    uint16_t _firstLoopingFrame;
    std::vector<uint16_t> _frameTimes;
    uint16_t _cycleTime;
    uint16_t _cycleTimeBeforeFirstLoopingFrame;
    std::vector<TextureRegion> _textureRegions;
};
