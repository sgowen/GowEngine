//
//  SteamServerInfo.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/13/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "deps/steamworks_sdk/Steam.hpp"

class SteamServerInfo
{
public:
    SteamServerInfo(gameserveritem_t *pGameServerItem);
    
    const char* getName();
    const char* getDisplayString();
    uint32 getIP();
    int32 getPort();
    CSteamID getSteamID();
    
private:
    uint32 _unIPAddress; // IP address for the server
    int32 _nConnectionPort; // Port for game clients to connect to for this server
    int _nPing; // current ping time in milliseconds
    char _szMap[32]; // current map
    char _szGameDescription[64]; // game description
    int _nPlayers; // current number of players on the server
    int _nMaxPlayers; // Maximum players that can join this server
    int _nBotPlayers; // Number of bots (i.e simulated players) on this server
    bool _bPassword; // true if this server needs a password to join
    bool _bSecure; // Is this server protected by VAC
    int	_nServerVersion; // server version as reported to Steam
    char _szServerName[64]; // Game server name
    char _szServerString[128]; // String to show in server browser
    CSteamID _steamID;
};

#endif /* IS_DESKTOP */
