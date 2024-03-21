//
//  LuaEngineState.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/21/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

void LuaEngineState::onEnter(Engine* e)
{
    // Empty
}

void LuaEngineState::onAssetsLoaded(Engine *e)
{
    // Empty
}

void LuaEngineState::onExit(Engine* e)
{
    // Empty
}

void LuaEngineState::onUpdate(Engine* e)
{
    // TODO
}

void LuaEngineState::onRender(Renderer& r)
{
    // TODO, support renderWorldWithLua as well
    r.renderWithLua();
}

LuaEngineState::LuaEngineState(std::string filePathAssets, bool areAssetsGlobal) : EngineState(filePathAssets, areAssetsGlobal)
{
    // Empty
}
