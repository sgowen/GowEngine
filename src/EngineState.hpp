//
//  EngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 12/27/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "State.hpp"

class Engine;

class EngineState : public State<Engine>
{
public:
    virtual void enter(Engine* e) = 0;
    virtual void execute(Engine* e) = 0;
    virtual void exit(Engine* e) = 0;
    
    virtual void createDeviceDependentResources() = 0;
    virtual void onWindowSizeChanged(int screenWidth, int screenHeight, int cursorWidth, int cursorHeight) = 0;
    virtual void releaseDeviceDependentResources() = 0;
    virtual void onResume() = 0;
    virtual void onPause() = 0;
    virtual void update() = 0;
    virtual void render(double alpha) = 0;

protected:
    EngineState() {}
    virtual ~EngineState() {}
};
