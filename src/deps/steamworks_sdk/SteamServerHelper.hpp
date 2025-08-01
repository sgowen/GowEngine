//
//  SteamServerHelper.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "core/network/ServerHelper.hpp"

#include "deps/steamworks_sdk/Steam.hpp"
#include "core/network/PacketHandler.hpp"
#include "core/common/Constants.hpp"

#include <string>

class SteamAddress;
class ClientProxy;

class SteamServerHelper : public ServerHelper
{
public:
    SteamServerHelper(std::string inGameDir,
                      std::string inVersionString,
                      std::string inProductName,
                      std::string inGameDescription,
                      uint16 inPort,
                      TimeTracker& tt,
                      ProcessPacketFunc inProcessPacketFunc,
                      GetClientProxyFunc inGetClientProxyFunc,
                      HandleClientDisconnectedFunc inHandleClientDisconnectedFunc);
    virtual ~SteamServerHelper();
    
    virtual int connect();
    virtual void processIncomingPackets();
    virtual void processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    virtual void onClientDisconnected(ClientProxy* clientProxy);
    virtual MachineAddress* getServerAddress();
    virtual bool isConnected();
    
    void kickPlayerOffServer(CSteamID steamID);
    
private:
    std::string _gameDir;
    std::string _versionString;
    std::string _productName;
    std::string _gameDescription;
    uint16 _port;
    SteamAddress* _serverSteamAddress;
    std::string _serverName;
    bool _isConnectedToSteam;
    bool _isServerReady;
    SteamAddress* _outgoingPacketAddress;
    
    struct ClientConnectionData_t
    {
        bool _isActive; // Is this slot in use? Or is it available for new connections?
        CSteamID _steamIDUser; // What is the steamid of the player?
    };
    
    // Vector to keep track of client connections which are pending auth
    ClientConnectionData_t _rgPendingClientData[MAX_NUM_PLAYERS];
    
    void sendUpdatedServerDetailsToSteam();
    void onClientBeginAuthentication(CSteamID steamIDClient, void* token, uint32 uTokenLen);
    void onAuthCompleted(bool bAuthSuccessful, uint32 iPendingAuthIndex);
    void sendDataToClient(CSteamID steamIDUser, const OutputMemoryBitStream& inOutputStream);
    
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onSteamServersConnected, SteamServersConnected_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onSteamServersConnectFailure, SteamServerConnectFailure_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onSteamServersDisconnected, SteamServersDisconnected_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onPolicyResponse, GSPolicyResponse_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onValidateAuthTicketResponse, ValidateAuthTicketResponse_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onP2PSessionRequest, P2PSessionRequest_t);
    STEAM_GAMESERVER_CALLBACK(SteamServerHelper, onP2PSessionConnectFail, P2PSessionConnectFail_t);
};

#endif /* IS_DESKTOP */
