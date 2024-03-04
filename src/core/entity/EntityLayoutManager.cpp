//
//  EntityLayoutManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/10/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EntityLayout& EntityLayoutManager::entityLayout(uint32_t key)
{
    auto q = _entityLayouts.find(key);
    assert(q != _entityLayouts.end());

    return q->second;
}

uint32_t EntityLayoutManager::getFirstLayout()
{
    return _entityLayouts.begin()->first;
}
