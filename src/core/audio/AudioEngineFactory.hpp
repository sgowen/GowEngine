//
//  AudioEngineFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class GowAudioEngine;

class AudioEngineFactory
{
public:
    static GowAudioEngine& getInstance();
    
private:
    AudioEngineFactory();
    ~AudioEngineFactory();
    AudioEngineFactory(const AudioEngineFactory&);
    AudioEngineFactory& operator=(const AudioEngineFactory&);
};
