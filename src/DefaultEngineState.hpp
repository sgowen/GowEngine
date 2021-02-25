//
//  DefaultEngineState.hpp
//  GGJ21
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "StateMachine.hpp"

class Engine;

#define ENGINE_STATE_DEFAULT DefaultEngineState::getInstance()

class DefaultEngineState : public State<Engine>
{
public:
    static DefaultEngineState& getInstance()
    {
        static DefaultEngineState ret = DefaultEngineState();
        return ret;
    }
    
    virtual void enter(Engine* e) {}
    virtual void execute(Engine* e);
    virtual void exit(Engine* e) {}
    
private:
    bool ERSA_CREATE_RESOURCES_called;
    bool ERSA_WINDOW_SIZE_CHANGED_called;
    
    DefaultEngineState();
    virtual ~DefaultEngineState() {}
    DefaultEngineState(const DefaultEngineState&);
    DefaultEngineState& operator=(const DefaultEngineState&);
};
