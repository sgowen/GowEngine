//
//  NosPhysicsWorld.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/physics/World.hpp"

class NosPhysicsWorld : public World
{
    DECL_RTTI;
    
public:
    NosPhysicsWorld();
    virtual ~NosPhysicsWorld();
    
    virtual void stepPhysics(float deltaTime) override;
    virtual void interpolatePhysics(float interpolation) override;
    virtual void endInterpolatedPhysics() override;
    
protected:
    virtual EntityPhysicsController* createPhysicsController(Entity* e) override;
    
private:
    float _gravity;
};
