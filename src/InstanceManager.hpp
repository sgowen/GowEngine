//
//  InstanceManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#define INSTANCE_MGR (InstanceManager::getInstance())

#include <map>

class InstanceManager
{
public:
    static InstanceManager& getInstance();
    
    void* get(uint32_t key);
    void registerInstance(uint32_t key, void* instance);
    
private:
    std::map<uint32_t, void*> _instanceMap;
    
    InstanceManager();
    ~InstanceManager();
    InstanceManager(const InstanceManager&);
    InstanceManager& operator=(const InstanceManager&);
};
