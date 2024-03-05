//
//  LuaRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

class Renderer;
class World;

namespace sol { class state; }

class LuaRenderer
{
public:
    LuaRenderer(Renderer& r);
    ~LuaRenderer();
    
    void render(Renderer& r, World& w, std::string script);
    
private:
    sol::state* _lua;
    std::map<std::string, Color> _colorMap;
    
    LuaRenderer(const LuaRenderer&);
    LuaRenderer& operator=(const LuaRenderer&);
};
