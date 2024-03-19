//
//  LuaUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/18/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

struct Script;
class Entity;
class Renderer;
class World;

namespace sol { class state; }

class LuaUtil
{
public:
    static LuaUtil& getInstance()
    {
        static LuaUtil ret = LuaUtil();
        return ret;
    }
    
    void loadScript(Script& s);
    void unloadScript(Script& s);
    
    void bindToEntity(sol::state& lua, Entity& e);
    void bindToRenderer(sol::state& lua, Renderer& r);
    void bindToWorld(sol::state& lua, World& w);
    void bindToRendererAndWorld(sol::state& lua, Renderer& r, World& w);
    
private:
    std::map<std::string, Color> _colorMap;
    
    LuaUtil();
    ~LuaUtil() {}
    LuaUtil(const LuaUtil&);
    LuaUtil& operator=(const LuaUtil&);
};
