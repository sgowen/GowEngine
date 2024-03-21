//
//  LuaEngineState.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 3/21/24.
//  Copyright © 2024 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/engine/EngineState.hpp"

class LuaEngineState : public EngineState
{
protected:
    virtual void onEnter(Engine* e) override;
    virtual void onAssetsLoaded(Engine* e) override;
    virtual void onExit(Engine* e) override;
    virtual void onUpdate(Engine* e) override;
    virtual void onRender(Renderer& r) override;
    
private:
    LuaEngineState(std::string filePathAssets, bool areAssetsGlobal = false);
    virtual ~LuaEngineState() {}
    LuaEngineState(const LuaEngineState&);
    LuaEngineState& operator=(const LuaEngineState&);
};
