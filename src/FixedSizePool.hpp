//
//  FixedSizePool.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 4/23/20.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <vector>

template <class T>
class FixedSizePool
{
public:
    FixedSizePool(int poolSize) :
    _poolSize(poolSize),
    _index(0)
    {
        _objectsPool.reserve(poolSize);
        
        for (int i = 0; i < _poolSize; ++i)
        {
            _objectsPool.push_back(T());
        }
    }
    
    virtual ~FixedSizePool()
    {
        _objectsPool.clear();
    }
    
    T* newObject()
    {
        T* obj = &_objectsPool.at(_index++);
        
        if (_index >= _poolSize)
        {
            _index = 0;
        }
        
        return obj;
    }
    
    void add(T* obj)
    {
        _objectsBuffer.push_back(obj);
    }
    
    void processBuffer()
    {
        _objects.clear();
        _objects.swap(_objectsBuffer);
        _objectsBuffer.clear();
    }
    
    std::vector<T*>& getObjects()
    {
        return _objects;
    }
    
    size_t getBufferSize()
    {
        return _objectsBuffer.size();
    }
    
private:
    std::vector<T*> _objects;
    std::vector<T> _objectsPool;
    std::vector<T*> _objectsBuffer;
    
    int _poolSize;
    int _index;
};
