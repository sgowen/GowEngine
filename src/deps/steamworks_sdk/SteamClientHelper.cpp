//
//  SteamClientHelper.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/17/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

#if IS_DESKTOP

SteamClientHelper::SteamClientHelper(CSteamID inServerSteamID,
                                     TimeTracker& tt,
                                     GetPlayerAddressHashFunc inGetPlayerAddressHashFunc,
                                     ProcessPacketFunc inProcessPacketFunc) : ClientHelper(new SteamPacketHandler(tt, false, inProcessPacketFunc)),
_timeTracker(tt),
_steamP2PAuth(new SteamP2PAuth(this)),
_getPlayerAddressHashFunc(inGetPlayerAddressHashFunc),
_eConnectedStatus(k_EClientNotConnected),
_serverSteamAddress(new SteamAddress(inServerSteamID)),
_timeOfLastMsgClientBeginAuthentication(0.0f),
_unServerIP(0),
_usServerPort(0),
_hAuthTicket(k_HAuthTicketInvalid),
_name(std::string(SteamFriends()->GetFriendPersonaName(SteamUser()->GetSteamID())))
{
    // Empty
}

SteamClientHelper::~SteamClientHelper()
{
    if (_hAuthTicket != k_HAuthTicketInvalid)
    {
        SteamUser()->CancelAuthTicket(_hAuthTicket);
    }

    OutputMemoryBitStream packet(ENGINE_CFG.mtuSize());
    packet.write(static_cast<uint8_t>(k_EMsgClientLeavingServer));
    sendPacketToServer(packet);

    _steamP2PAuth->endGame();

    delete _steamP2PAuth;
    _steamP2PAuth = nullptr;

    if (_eConnectedStatus == k_EClientConnectedAndAuthenticated)
    {
        SteamUser()->TerminateGameConnection(_unServerIP, _usServerPort);
    }

    if (_serverSteamAddress->getSteamID().IsValid())
    {
        SteamNetworking()->CloseP2PSessionWithUser(_serverSteamAddress->getSteamID());
    }

    delete _serverSteamAddress;
}

int SteamClientHelper::connect()
{
    LOG("Client %s is connecting to Game Server with Steam ID: %s", _name.c_str(), _serverSteamAddress->toString().c_str());

    STEAM_GAME_SERVICES->onServerJoined();
    
    return NO_ERROR;
}

void SteamClientHelper::processIncomingPackets()
{
    NetworkHelper::processIncomingPackets();

    // has the player list changed?
    if (NW_SRVR)
    {
        // if i am the server owner i need to auth everyone who wants to play
        // assume i am in slot 0, so start at slot 1
        for (uint32_t i = 1; i < MAX_NUM_PLAYERS; ++i)
        {
            CSteamID steamIDNew;
            steamIDNew.SetFromUint64(_getPlayerAddressHashFunc(i));
            if (steamIDNew == SteamUser()->GetSteamID())
            {
                LOG("Server player slot 0 is not server owner.");
            }
            else if (steamIDNew != _rgSteamIDPlayers[i])
            {
                if (_rgSteamIDPlayers[i].IsValid())
                {
                    _steamP2PAuth->playerDisconnect(i);
                }
                if (steamIDNew.IsValid())
                {
                    _steamP2PAuth->registerPlayer(i, steamIDNew);
                }
            }
        }
    }
    else
    {
        // i am just a client, i need to auth the game owner (slot 0)
        CSteamID steamIDNew;
        steamIDNew.SetFromUint64(_getPlayerAddressHashFunc(0));
        if (steamIDNew == SteamUser()->GetSteamID())
        {
            LOG("Server player slot 0 is not server owner.");
        }
        else if (steamIDNew != _rgSteamIDPlayers[0])
        {
            if (_rgSteamIDPlayers[0].IsValid())
            {
                LOG("Server player slot 0 has disconnected - but thats the server owner.");
                LOG("steamIDNew: %lld", steamIDNew.ConvertToUint64());
                _steamP2PAuth->playerDisconnect(0);
            }
            if (steamIDNew.IsValid())
            {
                _steamP2PAuth->startAuthPlayer(0, steamIDNew);
            }
        }
    }

    for (uint32_t i = 0; i < MAX_NUM_PLAYERS; ++i)
    {
        // Update steamid array with data from server
        // This is actually sort of hacky...
        // It only works because we know that the hash of a SteamAddress
        // object is actually the CSteamID in uint64 form
        _rgSteamIDPlayers[i].SetFromUint64(_getPlayerAddressHashFunc(i));
    }

    if (NW_SRVR)
    {
        // Now if we are the owner of the game, lets make sure all of our players are legit.
        // if they are not, we tell the server to kick them off
        // Start at 1 to skip myself
        for (uint8_t i = 1; i < MAX_NUM_PLAYERS; ++i)
        {
            if (_steamP2PAuth->_rgpP2PAuthPlayer[i] && !_steamP2PAuth->_rgpP2PAuthPlayer[i]->isAuthOk())
            {
                SteamServerHelper* helper = static_cast<SteamServerHelper*>(NW_SRVR->getServerHelper());
                helper->kickPlayerOffServer(_steamP2PAuth->_rgpP2PAuthPlayer[i]->_steamID);
            }
        }
    }
    else
    {
        // If we are not the owner of the game, lets make sure the game owner is legit
        // if he is not, we leave the game
        if (_steamP2PAuth->_rgpP2PAuthPlayer[0])
        {
            if (!_steamP2PAuth->_rgpP2PAuthPlayer[0]->isAuthOk())
            {
                // leave the game
                _eConnectedStatus = k_EServerIsNotAuthorized;
                LOG("k_EServerIsNotAuthorized");
                updateState();
            }
        }
    }
}

void SteamClientHelper::processSpecialPacket(uint8_t packetType, InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress)
{
    if (inFromAddress->getHash() == _serverSteamAddress->getHash())
    {
        switch (packetType)
        {
            case k_EMsgServerSendInfo:
            {
                if (_eConnectedStatus == k_EClientNotConnected)
                {
                    uint64_t steamIDGameServer;
                    bool bVACSecure;
                    std::string serverName;
                    inInputStream.read(steamIDGameServer);
                    inInputStream.read(bVACSecure);
                    inInputStream.readLarge(serverName);

                    onReceiveServerInfo(CSteamID((uint64)steamIDGameServer), bVACSecure, serverName.c_str());
                }
            }
                break;
            case k_EMsgServerPassAuthentication:
                _eConnectedStatus = k_EClientConnectedAndAuthenticated;
                LOG("k_EClientConnectedAndAuthenticated");
                // set information so our friends can join the lobby
                updateRichPresenceConnectionInfo();
                break;
            case k_EMsgServerFailAuthentication:
                LOG("Connection failure. Multiplayer authentication failed");
                _eConnectedStatus = k_EClientConnectionFailure;
                break;
            case k_EMsgServerExiting:
                LOG("Server Shutting Down");
                _eConnectedStatus = k_EServerShuttingDown;

                _steamP2PAuth->endGame();
            default:
                break;
        }
    }
    else
    {
        _steamP2PAuth->handleMessage(packetType, inInputStream);
    }

    updateState();
}

void SteamClientHelper::handleUninitialized()
{
    switch (_eConnectedStatus)
    {
        case k_EClientNotConnected:
        {
            OutputMemoryBitStream packet(ENGINE_CFG.mtuSize());
            packet.write(static_cast<uint8_t>(k_EMsgClientInitiateConnection));
            sendPacketToServer(packet);
        }
            break;
        case k_EClientConnectedPendingAuthentication:
        {
            float time = _timeTracker.realTime();

            if (time > _timeOfLastMsgClientBeginAuthentication + 7.0f)
            {
                char rgchToken[1024];
                uint32_t unTokenLen = 0;
                _hAuthTicket = SteamUser()->GetAuthSessionTicket(rgchToken, sizeof(rgchToken), &unTokenLen);

                Steamworks_TestSecret();

                if (unTokenLen < 1)
                {
                    LOG("Warning: Looks like GetAuthSessionTicket didn't give us a good ticket");
                }

                LOG("MsgClientBeginAuthentication_t, uTokenLen: %i", unTokenLen);

                OutputMemoryBitStream packet(ENGINE_CFG.mtuSize());
                packet.write(static_cast<uint8_t>(k_EMsgClientBeginAuthentication));
                packet.write(unTokenLen);
                packet.writeBytes(rgchToken, unTokenLen);
                sendPacketToServer(packet);

                _timeOfLastMsgClientBeginAuthentication = time;
            }
        }
            break;
        default:
            break;
    }

    updateState();
}

void SteamClientHelper::sendPacketToServer(const OutputMemoryBitStream& inOutputStream)
{
    NetworkHelper::sendPacket(inOutputStream, _serverSteamAddress);
}

std::string& SteamClientHelper::getName()
{
    return _name;
}

void SteamClientHelper::onReceiveServerInfo(CSteamID steamIDGameServer, bool bVACSecure, const char *pchServerName)
{
    _eConnectedStatus = k_EClientConnectedPendingAuthentication;
    LOG("k_EClientConnectedPendingAuthentication");

    _serverSteamAddress->setSteamID(steamIDGameServer);

    // look up the servers IP and Port from the connection
    P2PSessionState_t p2pSessionState;
    SteamNetworking()->GetP2PSessionState(steamIDGameServer, &p2pSessionState);
    _unServerIP = p2pSessionState.m_nRemoteIP;
    _usServerPort = p2pSessionState.m_nRemotePort;

    // set how to connect to the game server, using the Rich Presence API
    // this lets our friends connect to this game via their friends list
    updateRichPresenceConnectionInfo();
}

void SteamClientHelper::updateRichPresenceConnectionInfo()
{
    // connect string that will come back to us on the command line	when a friend tries to join our game
    char rgchConnectString[128];
    rgchConnectString[0] = 0;

    if (_eConnectedStatus == k_EClientConnectedAndAuthenticated && _unServerIP && _usServerPort)
    {
        // game server connection method
        STRING_FORMAT(rgchConnectString, "+connect %d:%d", _unServerIP, _usServerPort);
    }

    SteamFriends()->SetRichPresence("connect", rgchConnectString);
}

void SteamClientHelper::updateState()
{
    _state = _eConnectedStatus == k_EClientConnectedAndAuthenticated ? CLIENT_READY_TO_SAY_HELLO : (_eConnectedStatus == k_EClientConnectionFailure || _eConnectedStatus == k_EServerShuttingDown || _eConnectedStatus == k_EServerIsNotAuthorized) ? CLIENT_AUTH_FAILED : CLIENT_NOT_READY_TO_SAY_HELLO;
}

#endif /* IS_DESKTOP */
