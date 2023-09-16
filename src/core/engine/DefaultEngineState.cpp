//
//  DefaultEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/25/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void DefaultEngineState::execute(Engine* e)
{
    switch (e->requestedStateAction())
    {
        case ERSA_CREATE_RESOURCES:
            ERSA_CREATE_RESOURCES_called = true;
            break;
        case ERSA_WINDOW_SIZE_CHANGED:
            ERSA_WINDOW_SIZE_CHANGED_called = true;
            break;
        case ERSA_DEFAULT:
        default:
            break;
    }
    
    if (ERSA_CREATE_RESOURCES_called && ERSA_WINDOW_SIZE_CHANGED_called)
    {
        e->overwriteState(&e->_initialState);
    }
}

DefaultEngineState::DefaultEngineState() : State<Engine>(),
ERSA_CREATE_RESOURCES_called(false),
ERSA_WINDOW_SIZE_CHANGED_called(false)
{
    // Empty
}
