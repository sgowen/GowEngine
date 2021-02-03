//
//  EngineController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"
#include "PlatformMacros.hpp"

class Engine;

class EngineController
{
public:
    virtual State<Engine>* getInitialState() = 0;
    virtual double getFrameRate();
    
protected:
#if IS_ANDROID
    EngineController(void* data1, void* data2);
#else
    EngineController(void* data1 = NULL, void* data2 = NULL);
#endif
    virtual ~EngineController();
};
