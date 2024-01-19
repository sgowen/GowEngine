//
//  NGSteamAddress.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include <framework/network/portable/MachineAddress.hpp>

#include <framework/network/steam/NGSteam.hpp>

class NGSteamAddress : public MachineAddress
{
public:
    NGSteamAddress(CSteamID steamID);
    
    NGSteamAddress();
    
    virtual ~NGSteamAddress();
    
    virtual MachineAddress* createNewCopy();
    
    virtual uint64_t getHash() const;
    
    virtual std::string toString() const;
    
    void setSteamID(CSteamID steamID);
    
    CSteamID& getSteamID();
    
    void setReliable(bool isReliable);
    
    bool isReliable();
    
private:
    CSteamID _steamID;
    bool _isReliable;
};

#endif /* IS_DESKTOP */
