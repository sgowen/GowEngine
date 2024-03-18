//
//  ScriptManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/4/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/assets/ScriptLoader.hpp"
#include "Script.hpp"

#include <map>
#include <string>

class ScriptManager
{
public:
    void loadData(std::map<std::string, FileDescriptor>& sds);
    void loadIntoLuaAndFreeData(std::map<std::string, FileDescriptor>& sds);
    void reset();
    Script& script(std::string name);
    bool isScriptLoaded(std::string name);
    
private:
    ScriptLoader _loader;
    std::map<std::string, Script> _scripts;
};
