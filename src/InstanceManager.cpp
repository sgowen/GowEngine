//
//  InstanceManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "InstanceManager.hpp"

#include <assert.h>

InstanceManager& InstanceManager::getInstance()
{
    static InstanceManager ret = InstanceManager();
    return ret;
}

void* InstanceManager::get(uint32_t key)
{
    auto q = _instanceMap.find(key);
    
    assert(q != _instanceMap.end());
    
    return q->second;
}

void InstanceManager::registerInstance(uint32_t key, void* instance)
{
    _instanceMap[key] = instance;
}
