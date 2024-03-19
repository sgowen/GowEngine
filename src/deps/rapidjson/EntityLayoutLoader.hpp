//
//  EntityLayoutLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 2/27/21.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

class EntityIDManager;
class EntityLayout;
struct EntityLayout;

class EntityLayoutLoader
{
public:    
    static void loadEntityLayout(EntityLayout& el, EntityIDManager& eidm, bool isServer);
    static void saveEntityLayout(EntityLayout& el);
    
private:
    EntityLayoutLoader();
    ~EntityLayoutLoader();
    EntityLayoutLoader(const EntityLayoutLoader&);
    EntityLayoutLoader& operator=(const EntityLayoutLoader&);
};
