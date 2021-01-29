//
//  GowUtil.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 1/14/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <vector>
#include <map>

class GowUtil
{
public:
    template<typename T>
    static void cleanUpVectorOfPointers(std::vector<T*>& items)
    {
        for (typename std::vector<T*>::iterator i = items.begin(); i != items.end(); )
        {
            delete *i;
            
            i = items.erase(i);
        }
    }
    
    template<typename K, typename T>
    static void cleanUpMapOfPointers(std::map<K, T*>& items)
    {
        for (typename std::map<K, T*>::iterator i = items.begin(); i != items.end(); )
        {
            delete i->second;
            
            i = items.erase(i);
        }
    }
    
private:
    GowUtil();
    ~GowUtil();
    GowUtil(const GowUtil&);
    GowUtil& operator=(const GowUtil&);
};
