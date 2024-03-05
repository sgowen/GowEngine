//
//  ScriptLoader.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

struct Script;

class ScriptLoader
{
public:    
    void loadData(Script& s);
    void freeData(Script& s);
};
