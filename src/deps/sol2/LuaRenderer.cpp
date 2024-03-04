//
//  LuaRenderer.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#define SOL_USING_CXX_LUA 1
#include <sol/sol.hpp>

LuaRenderer::LuaRenderer(Renderer& r) : _lua(new sol::state())
{
    sol::state& lua = *_lua;
    
    lua.open_libraries(sol::lib::base);
    
    lua.set_function("updateMatrixCenteredOnPlayer", [&r] {
        r.updateMatrixCenteredOnPlayer();
    });
    
    lua.set_function("bindFramebuffer", [&r] {
        r.bindFramebuffer("main");
    });
    
    lua.set_function("clearFramebuffer", [&r] {
        r.clearFramebuffer(Color::GREEN);
    });
}

LuaRenderer::~LuaRenderer()
{
    delete _lua;
    _lua = nullptr;
}

void LuaRenderer::render(Renderer& r, World& w, std::string script)
{
    sol::state& lua = *_lua;
    
    lua.script(script);
}
