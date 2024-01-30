//
//  SteamClientHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "core/network/ClientHelper.hpp"

#include "core/network/PacketHandler.hpp"
#include "deps/steamworks_sdk/Steam.hpp"
#include "core/common/Constants.hpp"

class SteamP2PAuth;
class SteamAddress;

typedef uint64_t (*GetPlayerAddressHashFunc)(uint8_t inPlayerIndex);

class SteamClientHelper : public ClientHelper
{
public:
    SteamClientHelper(CSteamID serverSteamID,
                        TimeTracker& tt,
                        GetPlayerAddressHashFunc getPlayerAddressHashFunc,
                        ProcessPacketFunc processPacketFunc);
    virtual ~SteamClientHelper();
    
    virtual int connect();
    virtual void processIncomingPackets();
    virtual void processSpecialPacket(uint8_t packetType, 
                                      InputMemoryBitStream& inInputStream,
                                      MachineAddress* inFromAddress);
    virtual void handleUninitialized();
    virtual void sendPacket(const OutputMemoryBitStream& inOutputStream);
    virtual std::string& getName();
    
private:
    // Enum for various client connection states
    enum EClientConnectionState
    {
        // Initial state, not connected to a server
        k_EClientNotConnected,
        // We've established communication with the server, but it hasn't authed us yet
        k_EClientConnectedPendingAuthentication,
        // Final phase, server has authed us, we are actually able to play on it
        k_EClientConnectedAndAuthenticated,
        k_EClientConnectionFailure,
        k_EServerIsNotAuthorized,
        k_EServerShuttingDown
    };
    
    TimeTracker& _timeTracker;
    SteamP2PAuth* _steamP2PAuth;
    GetPlayerAddressHashFunc _getPlayerAddressHashFunc;
    EClientConnectionState _eConnectedStatus;
    SteamAddress* _serverSteamAddress;
    std::string _name;
    float _timeOfLastMsgClientBeginAuthentication;
    
    CSteamID _rgSteamIDPlayers[MAX_NUM_PLAYERS];
    
    // Server address data
    uint32_t _unServerIP;
    uint16 _usServerPort;
    HAuthTicket _hAuthTicket;
    
    void onReceiveServerInfo(CSteamID steamIDGameServer, bool bVACSecure, const char *pchServerName);
    void updateRichPresenceConnectionInfo();
    void updateState();
};

#endif /* IS_DESKTOP */
