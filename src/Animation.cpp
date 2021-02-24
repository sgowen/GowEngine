//
//  Animation.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "Animation.hpp"

#include "TextureRegion.hpp"

#include <assert.h>

Animation::Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool isLooping, int firstLoopingFrame, int xPadding, int yPadding, std::vector<uint16_t> frameTimes) :
_isLooping(isLooping),
_firstLoopingFrame(firstLoopingFrame),
_frameTimes(frameTimes),
_cycleTime(0)
{
    for (auto frameTime : _frameTimes)
    {
        _cycleTime += frameTime;
    }
    assert(_cycleTime > 0);

    const int numFrames = static_cast<int>(_frameTimes.size());
	const int right = x + animationWidth;
    const int bottom = y + animationHeight;

    for (int j = y; j < bottom; j += regionHeight + yPadding)
    {
        for (int i = x; i < right; i += regionWidth + xPadding)
        {
            _textureRegions.emplace_back(i, j, regionWidth, regionHeight, textureWidth, textureHeight);

            if (_textureRegions.size() == numFrames)
            {
                return;
            }
        }
    }
}

TextureRegion& Animation::textureRegion(uint16_t stateTime)
{
    return textureRegionAtKeyFrame(frameIndex(stateTime));
}

TextureRegion& Animation::textureRegionAtKeyFrame(uint16_t frameIndex)
{
	return _textureRegions.at(frameIndex);
}

uint16_t Animation::frameIndex(uint16_t stateTime)
{
    if (stateTime >= _cycleTime)
    {
        if (_isLooping)
        {
            uint16_t cycleTime = _cycleTime;
            if (_firstLoopingFrame > 0)
            {
                for (uint16_t i = 0; i < _firstLoopingFrame; ++i)
                {
                    cycleTime -= _frameTimes.at(i);
                }
            }

            stateTime %= cycleTime;
        }
        else
        {
            return static_cast<int>(_frameTimes.size()) - 1;
        }
    }

    for (uint16_t i = 0; i < _frameTimes.size(); ++i)
    {
        uint16_t frameTime = _frameTimes.at(i);

        if (stateTime < frameTime)
        {
            return i;
        }

        stateTime -= frameTime;
    }

    return 0;
}
