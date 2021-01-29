//
//  DirectXSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "DirectXSoundWrapper.hpp"

#include "DirectXSound.hpp"

#include "GowUtil.hpp"

DirectXSoundWrapper::DirectXSoundWrapper(uint16_t soundID, const char *filePath, DirectX::AudioEngine* audioEngine, int numInstances) : SoundWrapper(soundID, numInstances)
{
    wchar_t* wString = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, filePath, -1, wString, 4096);
    
    _sound = std::make_unique<DirectX::SoundEffect>(audioEngine, wString);
    
    delete wString;
    
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new DirectXSound(soundID, *_sound));
    }
}

DirectXSoundWrapper::~DirectXSoundWrapper()
{
	GowUtil::cleanUpVectorOfPointers(_sounds);

    if (_sound)
    {
        _sound.reset();
    }
}
