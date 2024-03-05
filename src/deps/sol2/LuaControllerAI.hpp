//
//  LuaControllerAI.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/04/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityControllerAI.hpp"

namespace sol { class state; }

class LuaControllerAI : public EntityControllerAI
{
    DECL_RTTI;
    DECL_EntityControllerAI_create;
    
public:
    LuaControllerAI(Entity* e);
    virtual ~LuaControllerAI();
    
    virtual void runAI(uint16_t& inputState) override;
    
private:
    sol::state* _lua;
};
