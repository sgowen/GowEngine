//
//  GameHostEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/StateMachine.hpp"

class Engine;

class GameHostEngineState : public State<Engine>
{    
public:
    static GameHostEngineState& getInstance()
    {
        static GameHostEngineState ret = GameHostEngineState();
        return ret;
    }
    
    virtual void enter(Engine* e);
    virtual void execute(Engine* e);
    virtual void exit(Engine* e);
    
private:
    GameHostEngineState() : State<Engine>() {}
    virtual ~GameHostEngineState() {}
    GameHostEngineState(const GameHostEngineState&);
    GameHostEngineState& operator=(const GameHostEngineState&);
};
