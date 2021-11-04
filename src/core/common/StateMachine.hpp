//
//  StateMachine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "Config.hpp"
#include "Constants.hpp"

#include <stack>
#include <cassert>

template <class T>
class State
{
public:
    Config _args;
    
    State() :
    _args(Config::EMPTY)
    {
        // Empty
    }
    virtual ~State() {}
    
    virtual void enter(T* owner) = 0;
    virtual void execute(T* owner) = 0;
    virtual void exit(T* owner) = 0;
};

template <class T>
class StateMachine
{
public:
    StateMachine(T* owner, State<T>* initialState) :
    _owner(owner)
    {
        _states.push(initialState);
    }
    
    void execute() const
    {
        if (_states.size() > 0)
        {
            _states.top()->execute(_owner);
        }
    }
    
    void changeState(State<T>* state, Config args = Config::EMPTY)
    {
        assert(state != nullptr);
        
        exit();
        
        if (args.getBool(ARG_OVERWRITE_STATE, false))
        {
            _states.pop();
        }
        
        _states.push(state);
        
        enter(args);
    }
    
    void revertToPreviousState()
    {
        exit();
        
        _states.pop();
        
        revert();
    }
    
    State<T>* getCurrentState() const
    {
        return _states.size() > 0 ? _states.top() : nullptr;
    }
    
private:
    T* _owner;
    
    std::stack<State<T>*> _states;
    
    void enter(Config args)
    {
        State<T>* currentState = getCurrentState();
        assert(currentState != nullptr);
        
        currentState->_args = args;
        currentState->enter(_owner);
    }
    
    void revert()
    {
        State<T>* currentState = getCurrentState();
        assert(currentState != nullptr);
        
        currentState->enter(_owner);
    }
    
    void exit()
    {
        State<T>* currentState = getCurrentState();
        assert(currentState != nullptr);
        
        currentState->exit(_owner);
    }
};
