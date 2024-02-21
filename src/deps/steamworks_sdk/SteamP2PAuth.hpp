//
//  SteamP2PAuth.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 6/20/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <GowEngine/BuildConstants.hpp>
#if IS_DESKTOP

#include "deps/steamworks_sdk/Steam.hpp"
#include "core/common/Constants.hpp"

class InputMemoryBitStream;
class NetworkHelper;
class SteamAddress;

class SteamP2PAuthPlayer;
class SteamP2PNetworkTransport;
class MsgP2PSendingTicket;

struct TimeTracker;

class SteamP2PAuth
{
public:
    SteamP2PAuthPlayer *_rgpP2PAuthPlayer[MAX_NUM_PLAYERS];
    MsgP2PSendingTicket *_rgpQueuedMessage[MAX_NUM_PLAYERS];
    SteamP2PNetworkTransport *_networkTransport;

    SteamP2PAuth(NetworkHelper* networkHelper);
    ~SteamP2PAuth();

    void playerDisconnect(uint8_t iSlot);
    void endGame();
    void startAuthPlayer(uint8_t iSlot, CSteamID steamID);
    void registerPlayer(uint8_t iSlot, CSteamID steamID);
    bool handleMessage(uint8_t packetType, InputMemoryBitStream& inInputStream);
    void internalinitPlayer(uint8_t iSlot, CSteamID steamID, bool bStartAuthProcess);
    
private:
    NetworkHelper* _networkHelper;
};

class SteamP2PAuthPlayer
{
public:
    CSteamID _steamID;

	SteamP2PAuthPlayer(NetworkHelper *networkHelper, SteamP2PNetworkTransport *pNetworkTransport);
	~SteamP2PAuthPlayer();

    void endGame();
	void initPlayer(CSteamID steamID);
	void startAuthPlayer();
	bool isAuthOk();
	bool handleMessage(MsgP2PSendingTicket* msg);

	STEAM_CALLBACK(SteamP2PAuthPlayer, OnBeginAuthResponse, ValidateAuthTicketResponse_t, _callbackBeginAuthResponse);

private:
    NetworkHelper* _networkHelper;
	bool _bSentTicket;
	bool _bSubmittedHisTicket;
	bool _bHaveAnswer;
	float _connectTime;
	float _ticketTime;
	float _answerTime;
	uint32_t _cubTicketIGaveThisUser;
	uint8_t _rgubTicketIGaveThisUser[1024];
	uint32_t _cubTicketHeGaveMe;
	uint8_t _rgubTicketHeGaveMe[1024];
	HAuthTicket _hAuthTicketIGaveThisUser;
	EBeginAuthSessionResult _eBeginAuthSessionResult;
	EAuthSessionResponse _eAuthSessionResponse;

	SteamP2PNetworkTransport *_networkTransport;
};

class SteamP2PNetworkTransport
{
public:
	SteamP2PNetworkTransport();
    ~SteamP2PNetworkTransport();

	void sendTicket(NetworkHelper* networkHelper, CSteamID steamIDFrom, CSteamID steamIDTo, uint32_t cubTicket, uint8_t *pubTicket);
	void closeConnection(CSteamID steamID);

	STEAM_CALLBACK(SteamP2PNetworkTransport, onP2PSessionRequest, P2PSessionRequest_t, _CallbackP2PSessionRequest);
	STEAM_CALLBACK(SteamP2PNetworkTransport, onP2PSessionConnectFail, P2PSessionConnectFail_t, _CallbackP2PSessionConnectFail);

private:
    SteamAddress* _outgoingPacketAddress;
};

class MsgP2PSendingTicket
{
public:
    MsgP2PSendingTicket();

    uint8_t getMessageType();
    void setToken(const char *pchToken, uint32_t unLen);
    uint32_t getTokenLen();
    const char* getTokenPtr();
    void setSteamID(uint64_t ulSteamID);
    uint64_t getSteamID();

private:
    const uint8_t _messageType;
    uint32_t _uTokenLen;
    char _rgchToken[1024];
    uint64_t _ulSteamID;
};

#endif /* IS_DESKTOP */
