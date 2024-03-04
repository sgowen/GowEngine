//
//  EntityInputMappingManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

EntityInputMapping& EntityInputMappingManager::entityInputMapping(std::string key)
{
    auto q = _entityInputMappings.find(key);
    assert(q != _entityInputMappings.end());

    return q->second;
}
