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

void LuaRenderer::renderWorld(Renderer& r, World& w, std::string script)
{
    Script& s = ASSETS_MGR.script(script);
    sol::state& lua = *s._lua;
    
    LUA.bindToRenderer(lua, r);
    LUA.bindToWorld(lua, w);
    LUA.bindToRendererAndWorld(lua, r, w);
    
    lua.set_function("numRollbackFrames", [&r]() {
        // TODO, this requires LuaRenderer to be a friend of Renderer
        return r._rc.numRollbackFrames;
    });
    
    lua["render"]();
}

void LuaRenderer::render(Renderer& r, std::string script)
{
    Script& s = ASSETS_MGR.script(script);
    sol::state& lua = *s._lua;
    
    LUA.bindToRenderer(lua, r);
    
    lua["render"]();
}
