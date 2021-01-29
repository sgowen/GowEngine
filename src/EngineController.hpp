//
//  EngineController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class EngineState;

class EngineController
{
public:
    virtual EngineState* getInitialState() = 0;
    
protected:
    EngineController() {}
    virtual ~EngineController() {}
};
