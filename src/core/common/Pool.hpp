//
//  Pool.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 11/24/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "STLUtil.hpp"

#include <cassert>
#include <vector>
#include <algorithm>

template <typename T>
class Pool
{
public:    
    ~Pool()
    {
        STLUtil::cleanUpVectorOfPointers(_objects);
    }
    
    T* obtain()
    {
        if (_objects.size() > 0)
        {
            T* ret = _objects.front();
            _objects.erase(_objects.begin());
            
            return ret;
        }
        else
        {
            return new T();
        }
    }
    
    void free(T* object)
    {
        assert(object != nullptr);
        assert(std::find(_objects.begin(), _objects.end(), object) == _objects.end());
        
        _objects.push_back(object);
    }
    
private:
    std::vector<T*> _objects;
};
