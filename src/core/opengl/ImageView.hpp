//
//  ImageView.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/3/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>

enum ImageVisibility
{
    IMGV_HIDDEN = 0,
    IMGV_VISIBLE
};

struct ImageView
{
    std::string _texture;
    std::string _textureRegion;
    float _xWeight;
    float _yWeight;
    float _widthWeight;
    float _heightWeight;
    uint8_t _visibility;
    
    ImageView(std::string texture, std::string textureRegion, float xWeight, float yWeight, float widthWeight, float heightWeight) :
    _texture(texture),
    _textureRegion(textureRegion),
    _xWeight(xWeight),
    _yWeight(yWeight),
    _widthWeight(widthWeight),
    _heightWeight(heightWeight),
    _visibility(IMGV_VISIBLE)
    {
        // Empty
    }
};
