//
//  NGSteamClientHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include <framework/network/client/ClientHelper.hpp>

#include <framework/network/portable/PacketHandler.hpp>
#include <framework/network/steam/NGSteam.hpp>
#include <framework/util/Constants.hpp>

class NGSteamP2PAuth;
class NGSteamAddress;

typedef uint64_t (*GetPlayerAddressHashFunc)(uint8_t inPlayerIndex);

class NGSteamClientHelper : public ClientHelper
{
public:
    NGSteamClientHelper(CSteamID serverSteamID, GetPlayerAddressHashFunc getPlayerAddressHashFunc, ProcessPacketFunc processPacketFunc, HandleNoResponseFunc handleNoResponseFunc, HandleConnectionResetFunc handleConnectionResetFunc);
    virtual ~NGSteamClientHelper();
    
    virtual void processIncomingPackets();
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    virtual void handleUninitialized();
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream);
    virtual std::string& getName();
    
private:
    // Enum for various client connection states
    enum EClientConnectionState
    {
        k_EClientNotConnected,							// Initial state, not connected to a server
        k_EClientConnectedPendingAuthentication,		// We've established communication with the server, but it hasn't authed us yet
        k_EClientConnectedAndAuthenticated,				// Final phase, server has authed us, we are actually able to play on it
        k_EClientConnectionFailure,
        k_EServerIsNotAuthorized,
        k_EServerShuttingDown
    };
    
    NGSteamP2PAuth* _steamP2PAuth;
    GetPlayerAddressHashFunc _getPlayerAddressHashFunc;
    EClientConnectionState _eConnectedStatus;
    NGSteamAddress* _serverSteamAddress;
    std::string _name;
    float _timeOfLastMsgClientBeginAuthentication;
    
    CSteamID _rgSteamIDPlayers[MAX_NUM_PLAYERS_PER_SERVER];
    
    // Server address data
    uint32_t _unServerIP;
    uint16 _usServerPort;
    HAuthTicket _hAuthTicket;
    
    void onReceiveServerInfo(CSteamID steamIDGameServer, bool bVACSecure, const char *pchServerName);
    void updateRichPresenceConnectionInfo();
    void updateState();
};

#endif /* IS_DESKTOP */
