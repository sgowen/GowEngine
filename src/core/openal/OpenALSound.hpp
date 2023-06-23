//
//  OpenALSound.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class OpenALSound
{
public:
    uint32_t _buf;
    uint32_t _src;
    bool _isLooping;
    
    OpenALSound(std::string filePath, float volume = 1.0f);
    ~OpenALSound();
};
