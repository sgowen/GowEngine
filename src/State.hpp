//
//  State.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 8/28/15.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

template <class ObjectType>
class State
{
public:
    State()
    {
        // Empty
    }
    
    virtual ~State()
    {
        // Empty
    }
    
    virtual void enter(ObjectType* owner) = 0;
    virtual void execute(ObjectType* owner) = 0;
    virtual void exit(ObjectType* owner) = 0;
};
