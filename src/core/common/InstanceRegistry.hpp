//
//  InstanceRegistry.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <cassert>

class InstanceRegistry
{
public:
    static InstanceRegistry& getInstance()
    {
        static InstanceRegistry ret = InstanceRegistry();
        return ret;
    }
    
    template<typename T>
    T* get(uint32_t key)
    {
        auto q = _instances.find(key);
        
        assert(q != _instances.end());
        
        void* ret = q->second;
        
        return static_cast<T*>(ret);
    }
    
    void registerInstance(uint32_t key, void* instance)
    {
        _instances[key] = instance;
    }
    
private:
    std::map<uint32_t, void*> _instances;
    
    InstanceRegistry() {}
    ~InstanceRegistry() {}
    InstanceRegistry(const InstanceRegistry&);
    InstanceRegistry& operator=(const InstanceRegistry&);
};
