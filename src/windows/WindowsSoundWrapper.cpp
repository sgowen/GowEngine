//
//  WindowsSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsSoundWrapper.hpp"

#include "WindowsSound.hpp"

#include "GowUtil.hpp"

WindowsSoundWrapper::WindowsSoundWrapper(uint16_t soundID, const char *filePath, Windows::AudioEngine* audioEngine, int numInstances) : SoundWrapper(soundID, numInstances)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, filePath, -1, wString, 4096);
    
    _sound = std::make_unique<Windows::SoundEffect>(audioEngine, wString);
    
    delete wString;
    
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new WindowsSound(soundID, *_sound));
    }
}

WindowsSoundWrapper::~WindowsSoundWrapper()
{
	GowUtil::cleanUpVectorOfPointers(_sounds);

    if (_sound)
    {
        _sound.reset();
    }
}
