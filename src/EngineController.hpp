//
//  EngineController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"

class Engine;

class EngineController
{
public:
    virtual State<Engine>* getInitialState() = 0;
    virtual double getFrameRate() { return (1.0 / 60.0); } // default is 60 FPS
    
protected:
    EngineController() {}
    virtual ~EngineController() {}
};
