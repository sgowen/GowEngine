//
//  DefaultEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/StateMachine.hpp"

class Engine;

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
