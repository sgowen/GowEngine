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
    void loadData(std::map<std::string, FileDescriptor>& scriptDescriptors);
    // TODO, it'd be awesome to just load up all the lua scripts and create
    // all the sol::state instances we'd need up front,
    // and just invoke them as needed
//    void loadIntoLuaAndFreeData(std::vector<FileDescriptor>& scriptDescriptors);
    void reset();
    Script& script(std::string name);
    bool isScriptLoaded(std::string name);
    
private:
    ScriptLoader _loader;
    std::map<std::string, Script> _scripts;
};
