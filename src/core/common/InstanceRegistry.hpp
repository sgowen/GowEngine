//
//  InstanceRegistry.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/28/18.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <string>
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
    T* get(std::string key)
    {
        auto q = _instances.find(key);
        
        assert(q != _instances.end());
        
        void* ret = q->second;
        
        return static_cast<T*>(ret);
    }
    
    void registerInstance(void* instance, std::string key)
    {
        _instances[key] = instance;
    }
    
private:
    std::map<std::string, void*> _instances;
    
    InstanceRegistry() {}
    ~InstanceRegistry() {}
    InstanceRegistry(const InstanceRegistry&);
    InstanceRegistry& operator=(const InstanceRegistry&);
};
