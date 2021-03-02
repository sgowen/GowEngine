//
//  Macros.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define UNUSED(x) (void)(x)
#define BUFFER_OFFSET(i) ((void*)(i))
#define SET_BIT(val, bit, set) val = (val & ~bit) | (set ? bit : 0)
#define IS_BIT_SET(val, bit) (val & bit)

#define DECL_EntityController_create(type)      \
public:                                         \
    static type* create(Entity* e)

#define IMPL_EntityController_create(name,type) \
type* name::create(Entity* e)                   \
{                                               \
    return new name(e);                         \
}