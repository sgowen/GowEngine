//
//  MonsterAIController.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityAIController.hpp"

class MonsterAIController : public EntityAIController
{
    DECL_RTTI;
    DECL_EntityAIController_create;
    
public:
    MonsterAIController(Entity* e);
    virtual ~MonsterAIController() {}
    
    virtual void runAI(uint16_t& inputState) override;
};
