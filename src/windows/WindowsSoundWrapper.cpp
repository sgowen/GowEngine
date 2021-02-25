//
//  WindowsSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "WindowsSoundWrapper.hpp"

#include "WindowsSound.hpp"
#include "STLUtil.hpp"

WindowsSoundWrapper::WindowsSoundWrapper(const char *filePath, DirectX::AudioEngine* audioEngine, uint8_t numInstances) : SoundWrapper(numInstances)
{
    wchar_t* wFilePath = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, filePath, -1, wFilePath, 4096);
    
    _sound = std::make_unique<DirectX::SoundEffect>(audioEngine, wFilePath);
    
    delete wFilePath;
    
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new WindowsSound(*_sound));
    }
}

WindowsSoundWrapper::~WindowsSoundWrapper()
{
	STLUtil::cleanUpVectorOfPointers(_sounds);

    _sound.reset();
}
