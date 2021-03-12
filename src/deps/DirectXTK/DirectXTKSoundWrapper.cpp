//
//  DirectXTKSoundWrapper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>
#if IS_WINDOWS

DirectXTKSoundWrapper::DirectXTKSoundWrapper(std::string filePath, DirectX::AudioEngine* audioEngine, uint8_t numInstances) : SoundWrapper(numInstances)
{
    wchar_t* wFilePath = new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, filePath, -1, wFilePath, 4096);
    
    _sound = std::make_unique<DirectX::SoundEffect>(audioEngine, wFilePath);
    
    delete wFilePath;
    
    for (int i = 0; i < _numInstances; ++i)
    {
        _sounds.push_back(new DirectXTKSound(*_sound));
    }
}

DirectXTKSoundWrapper::~DirectXTKSoundWrapper()
{
	STLUtil::cleanUpVectorOfPointers(_sounds);

    _sound.reset();
}

#endif /* IS_WINDOWS */
