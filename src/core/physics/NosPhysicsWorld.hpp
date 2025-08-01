//
//  NosPhysicsWorld.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/29/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/World.hpp"

class NosPhysicsWorld : public World
{
    DECL_RTTI;
    
public:
    NosPhysicsWorld();
    virtual ~NosPhysicsWorld();
    
    virtual void stepPhysics(float deltaTime) override;
    virtual void extrapolatePhysics(float extrapolation) override;
    virtual void endExtrapolatedPhysics() override;
    
protected:
    virtual EntityControllerPhysics* createControllerPhysics(Entity* e) override;
};
