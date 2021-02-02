//
//  StateMachine.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <stack>
#include <assert.h>

template <class T>
class State
{
public:
    State() {}
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
    
    ~StateMachine() {}
    
    void execute() const
    {
        if (_states.size() > 0)
        {
            _states.top()->execute(_owner);
        }
    }
    
    void changeState(State<T>* newState, bool overwrite = false)
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
    
    State<T>* getCurrentState() const
    {
        return _states.size() > 0 ? _states.top() : NULL;
    }
    
private:
    T* _owner;
    
    std::stack<State<T>*> _states;
    
    void enter()
    {
        State<T>* currentState = getCurrentState();
        assert(currentState != NULL);
        
        currentState->enter(_owner);
    }
    
    void exit()
    {
        State<T>* currentState = getCurrentState();
        assert(currentState != NULL);
        
        currentState->exit(_owner);
    }
};
