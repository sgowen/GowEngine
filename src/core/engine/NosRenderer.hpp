//
//  NosRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

class Renderer;
class World;
class Entity;

class NosRenderer
{
public:
    static void render(Renderer& r, World& w);
    
private:
    NosRenderer() {}
    ~NosRenderer() {}
    NosRenderer(const NosRenderer&);
    NosRenderer& operator=(const NosRenderer&);
};
