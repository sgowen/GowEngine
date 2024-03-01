//
//  DanteRenderer.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/1/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

class Renderer;
class World;

class DanteRenderer
{
public:
    static void render(Renderer& r, World& w, Entity* controlledPlayer, float scale);
    
private:
    DanteRenderer() {}
    ~DanteRenderer() {}
    DanteRenderer(const DanteRenderer&);
    DanteRenderer& operator=(const DanteRenderer&);
};
