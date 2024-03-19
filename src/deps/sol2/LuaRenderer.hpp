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

class LuaRenderer
{
public:
    LuaRenderer();
    ~LuaRenderer() {}
    
    void renderWorld(Renderer& r, World& w, std::string script);
    void render(Renderer& r, std::string script);
    
private:
    std::map<std::string, Color> _colorMap;
    
    LuaRenderer(const LuaRenderer&);
    LuaRenderer& operator=(const LuaRenderer&);
};
