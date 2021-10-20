//
//  NosPhysicsController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/04/21.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityPhysicsController.hpp"

#include <vector>

class Entity;

class NosPhysicsController : public EntityPhysicsController
{
    DECL_RTTI;
    
public:
    NosPhysicsController(Entity* e);
    virtual ~NosPhysicsController();
    
    virtual Vector2 velocity() override;
    virtual void setVelocity(Vector2 v) override;
    virtual void updatePoseFromBody() override;
    virtual void updateBodyFromPose() override;
    
private:
    bool _isBodyFacingLeft;
};
