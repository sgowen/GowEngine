//
//  OpenALSound.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 7/7/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

OpenALSound::OpenALSound(std::string filePath, float volume) :
_buf(0),
_src(0),
_isLooping(false)
{
    // TODO Consider refactoring to match architecture of TextureManager/TextureLoader
    _buf = SoundLoader::loadSound(filePath.c_str());
    assert(_buf != 0);

    OAL.create(*this);
}

OpenALSound::~OpenALSound()
{
    OAL.destroy(*this);
}
