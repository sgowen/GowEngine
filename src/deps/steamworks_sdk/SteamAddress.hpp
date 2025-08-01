//
//  SteamAddress.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "core/network/MachineAddress.hpp"

#include "deps/steamworks_sdk/Steam.hpp"

class SteamAddress : public MachineAddress
{
    DECL_RTTI;
    
public:
    SteamAddress(CSteamID steamID);
    SteamAddress();
    virtual ~SteamAddress();
    
    virtual MachineAddress* createNewCopy() override;
    virtual uint64_t getHash() const override;
    virtual std::string toString() const override;
    
    void setSteamID(CSteamID steamID);
    CSteamID& getSteamID();
    void setReliable(bool isReliable);
    bool isReliable();
    
private:
    CSteamID _steamID;
    bool _isReliable;
};

#endif /* IS_DESKTOP */
