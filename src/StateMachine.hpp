//
//  StateMachine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "State.hpp"

#include <stack>
#include <assert.h>

template <class ObjectType, class StateType>
class StateMachine
{
public:
    StateMachine(ObjectType* owner) :
    _owner(owner),
    _globalState(NULL)
    {
        // Empty
    }
    
    ~StateMachine() {}
    
    void execute() const
    {
        if (_globalState)
        {
            _globalState->execute(_owner);
        }
        
        if (_states.size() > 0)
        {
            _states.top()->execute(_owner);
        }
    }
    
    void changeState(StateType* newState, bool overwrite = false)
    {
        assert(newState != NULL);
        
        exit();
        
        if (overwrite)
        {
            _states.pop();
        }
        
        _states.push(newState);
        
        enter();
    }
    
    void revertToPreviousState()
    {
        exit();
        
        _states.pop();
        
        enter();
    }
    
    void setCurrentState(StateType* state)
    {
        while (!_states.empty())
        {
            _states.pop();
        }
        
        _states.push(state);
    }
    
    StateType* getCurrentState() const
    {
        return _states.size() > 0 ? _states.top() : NULL;
    }
    
    void setGlobalState(StateType* state)
    {
        _globalState = state;
    }
    
    StateType* getGlobalState() const
    {
        return _globalState;
    }
    
private:
    ObjectType* _owner;
    
    StateType* _globalState;
    std::stack<StateType*> _states;
    
    void enter()
    {
        StateType* currentState = getCurrentState();
        assert(currentState != NULL);
        
        currentState->enter(_owner);
    }
    
    void exit()
    {
        StateType* currentState = getCurrentState();
        assert(currentState != NULL);
        
        currentState->exit(_owner);
    }
};
