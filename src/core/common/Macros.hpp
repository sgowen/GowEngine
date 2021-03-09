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

#define DECL_EntityController_create(parent)        \
public:                                             \
    static parent* create(Entity* e)

#define IMPL_EntityController_create(type, parent)  \
parent* type::create(Entity* e)                     \
{                                                   \
    return new type(e);                             \
}

#define IMPL_EntityController_NOPARENT_create(type) \
type* type::create(Entity* e)                       \
{                                                   \
    return new type(e);                             \
}
