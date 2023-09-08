//
//  GameHostEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/27/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void GameHostEngineState::enter(Engine* e)
{
    ENGINE_STATE_GAME_SRVR.enter(e);
    ENGINE_STATE_GAME_CLNT._args = _args;
    ENGINE_STATE_GAME_CLNT.enter(e);
}

void GameHostEngineState::execute(Engine* e)
{
    ENGINE_STATE_GAME_SRVR.execute(e);
    ENGINE_STATE_GAME_CLNT.execute(e);
}

void GameHostEngineState::exit(Engine* e)
{
    ENGINE_STATE_GAME_CLNT.exit(e);
    ENGINE_STATE_GAME_SRVR.exit(e);
}
