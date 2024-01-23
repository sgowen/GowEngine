//
//  NGSteamAddress.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

IMPL_RTTI(NGSteamAddress, MachineAddress);

NGSteamAddress::NGSteamAddress(CSteamID steamID) : MachineAddress(),
_steamID(steamID),
_isReliable(false)
{
    // Empty
}

NGSteamAddress::NGSteamAddress() : MachineAddress(), 
_steamID(CSteamID()),
_isReliable(false)
{
    // Empty
}

NGSteamAddress::~NGSteamAddress()
{
    // Empty
}

MachineAddress* NGSteamAddress::createNewCopy()
{
    return new NGSteamAddress(_steamID);
}

uint64_t NGSteamAddress::getHash() const
{
    return _steamID.ConvertToUint64();
}

std::string NGSteamAddress::toString() const
{
    return StringUtil::format("Steam ID %llu", _steamID.ConvertToUint64());
}

void NGSteamAddress::setSteamID(CSteamID steamID)
{
    _steamID = steamID;
}

CSteamID& NGSteamAddress::getSteamID()
{
    return _steamID;
}

void NGSteamAddress::setReliable(bool isReliable)
{
    _isReliable = isReliable;
}

bool NGSteamAddress::isReliable()
{
    return _isReliable;
}

#endif /* IS_DESKTOP */
