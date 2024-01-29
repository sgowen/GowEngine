//
//  SteamServerInfo.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/13/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

SteamServerInfo::SteamServerInfo(gameserveritem_t *pGameServerItem)
{
    _unIPAddress = pGameServerItem->m_NetAdr.GetIP();
    _nConnectionPort = pGameServerItem->m_NetAdr.GetConnectionPort();
    _nPing = pGameServerItem->m_nPing;
    
    strncpy(_szMap, pGameServerItem->m_szMap, ARRAYSIZE(_szMap));
    strncpy(_szGameDescription, pGameServerItem->m_szGameDescription, ARRAYSIZE(_szGameDescription));
    
    _nPlayers = pGameServerItem->m_nPlayers;
    _nMaxPlayers = pGameServerItem->m_nMaxPlayers;
    _nBotPlayers = pGameServerItem->m_nBotPlayers;
    _bPassword = pGameServerItem->m_bPassword;
    _bSecure = pGameServerItem->m_bSecure;
    _nServerVersion = pGameServerItem->m_nServerVersion;
    
    strncpy(_szServerName, pGameServerItem->GetName(), ARRAYSIZE(_szServerName));
    
    STRING_FORMAT(_szServerString, "%s (%i/%i) at %s ping(%d)", pGameServerItem->GetName(), pGameServerItem->m_nPlayers, pGameServerItem->m_nMaxPlayers, pGameServerItem->m_NetAdr.GetConnectionAddressString(), pGameServerItem->m_nPing);
    
    _steamID = pGameServerItem->m_steamID;
}

const char* SteamServerInfo::getName()
{
    return _szServerName;
}

const char* SteamServerInfo::getDisplayString()
{
    return _szServerString;
}

uint32 SteamServerInfo::getIP()
{
    return _unIPAddress;
}

int32 SteamServerInfo::getPort()
{
    return _nConnectionPort;
}

CSteamID SteamServerInfo::getSteamID()
{
    return _steamID;
}

#endif /* IS_DESKTOP */
