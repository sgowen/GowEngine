//
//  AudioEngineHelperFactory.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/8/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class AudioEngineHelper;

class AudioEngineHelperFactory
{
public:
    static AudioEngineHelper* create();
    static void destroy(AudioEngineHelper* ah);
};
