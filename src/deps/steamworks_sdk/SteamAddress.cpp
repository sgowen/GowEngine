//
//  SteamAddress.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

IMPL_RTTI(SteamAddress, MachineAddress);

SteamAddress::SteamAddress(CSteamID steamID) : MachineAddress(),
_steamID(steamID),
_isReliable(false)
{
    // Empty
}

SteamAddress::SteamAddress() : MachineAddress(), 
_steamID(CSteamID()),
_isReliable(false)
{
    // Empty
}

SteamAddress::~SteamAddress()
{
    // Empty
}

MachineAddress* SteamAddress::createNewCopy()
{
    return new SteamAddress(_steamID);
}

uint64_t SteamAddress::getHash() const
{
    return _steamID.ConvertToUint64();
}

std::string SteamAddress::toString() const
{
    return StringUtil::format("Steam ID %llu", _steamID.ConvertToUint64());
}

void SteamAddress::setSteamID(CSteamID steamID)
{
    _steamID = steamID;
}

CSteamID& SteamAddress::getSteamID()
{
    return _steamID;
}

void SteamAddress::setReliable(bool isReliable)
{
    _isReliable = isReliable;
}

bool SteamAddress::isReliable()
{
    return _isReliable;
}

#endif /* IS_DESKTOP */
