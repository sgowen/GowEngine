//
//  Animation.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

Animation::Animation(uint16_t x, uint16_t y, std::vector<uint16_t> regionWidths, std::vector<uint16_t> regionHeights, uint16_t animationWidth, uint16_t animationHeight, uint16_t textureWidth, uint16_t textureHeight, bool isLooping, uint16_t firstLoopingFrame, uint16_t xPadding, uint16_t yPadding, std::vector<uint16_t> frameTimes) :
_isLooping(isLooping),
_firstLoopingFrame(firstLoopingFrame),
_frameTimes(frameTimes),
_cycleTime(0),
_cycleTimeBeforeFirstLoopingFrame(0)
{
    for (int i = 0; i < _frameTimes.size(); ++i)
    {
        if (i == _firstLoopingFrame)
        {
            _cycleTimeBeforeFirstLoopingFrame = _cycleTime;
        }
        
        auto frameTime = _frameTimes[i];
        _cycleTime += frameTime;
    }
    assert(_cycleTime > 0);

    const size_t numFrames = _frameTimes.size();
	const int right = x + animationWidth;
    const int bottom = y + animationHeight;

    int frame = 0;
    int xIncrement = 0;
    int yIncrement = 0;
    for (int j = y; j < bottom; j += yIncrement)
    {
        for (int i = x; i < right; i += xIncrement)
        {
            _textureRegions.emplace_back(i, j, regionWidths[frame], regionHeights[frame], textureWidth, textureHeight);
            xIncrement = regionWidths[frame] + xPadding;
            yIncrement = regionHeights[frame] + yPadding;
            ++frame;

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
    uint16_t i = 0;
    
    if (stateTime >= _cycleTime)
    {
        if (_isLooping)
        {
            uint16_t cycleTime = _cycleTime;
            if (_firstLoopingFrame > 0)
            {
                for ( ; i < _firstLoopingFrame; ++i)
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

    for ( ; i < _frameTimes.size(); ++i)
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

uint16_t Animation::cycleTime()
{
    return _cycleTime;
}

uint16_t Animation::cycleTimeBeforeFirstLoopingFrame()
{
    return _cycleTimeBeforeFirstLoopingFrame;
}

bool Animation::isLooping()
{
    return _isLooping;
}
