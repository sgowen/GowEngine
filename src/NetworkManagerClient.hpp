//
//  NetworkManagerClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "PacketHandler.hpp"
#include "EntityRegistry.hpp"
#include "DeliveryNotificationManager.hpp"
#include "ReplicationManagerClient.hpp"
#include "WeightedTimedMovingAverage.hpp"

#include <map>
#include <string>
#include <vector>

class InputMemoryBitStream;
class OutputMemoryBitStream;
class SocketAddress;
class Entity;
class MoveList;
class SocketAddress;

#define NW_MGR_CLNT (NetworkManagerClient::getInstance())

typedef void (*RemoveProcessedMovesFunc)(float lastMoveProcessedByServerTimestamp);
typedef MoveList& (*GetMoveListFunc)();
typedef void (*OnPlayerWelcomedFunc)(uint8_t playerID);

enum NetworkClientState
{
    NWCS_SAYING_HELLO,
    NWCS_WELCOMED,
    NWCS_DISCONNECTED
};

class NetworkManagerClient
{
public:
    static void create(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf);
    static NetworkManagerClient* getInstance();
    static void destroy();
    
    void processIncomingPackets();
    void sendOutgoingPackets();
    void requestToAddLocalPlayer();
    void requestToDropLocalPlayer(uint8_t index);
    const WeightedTimedMovingAverage& getBytesReceivedPerSecond() const;
    const WeightedTimedMovingAverage& getBytesSentPerSecond() const;
    const WeightedTimedMovingAverage& getAvgRoundTripTime()	const;
    float getRoundTripTime() const;
    bool isPlayerIDLocal(uint8_t playerID) const;
    bool hasReceivedNewState();
    std::map<uint8_t, uint8_t>& getPlayerIDs();
    std::string& getPlayerName();
    NetworkClientState state() const;
    bool isConnected();
    EntityRegistry& getEntityRegistry();
    void processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
    
private:
    static NetworkManagerClient* s_instance;
    
    PacketHandler _packetHandler;
    SocketAddress* _serverAddress;
    std::string _username;
    RemoveProcessedMovesFunc _removeProcessedMovesFunc;
    GetMoveListFunc _getMoveListFunc;
    OnPlayerWelcomedFunc _onPlayerWelcomedFunc;
    DeliveryNotificationManager _deliveryNotificationManager;
    EntityRegistry _entityRegistry;
    ReplicationManagerClient _replicationManagerClient;
    NetworkClientState _state;
    uint32_t _timeOfLastHello;
    std::map<uint8_t, uint8_t> _indexToPlayerIDMap;
    uint8_t _nextIndex;
    uint32_t _lastMoveProcessedByServerTimestamp;
    uint32_t _lastServerCommunicationTimestamp;
    WeightedTimedMovingAverage _avgRoundTripTime;
    bool _isRequestingToAddLocalPlayer;
    uint8_t _isRequestingToDropLocalPlayer;
    bool _hasReceivedNewState;
    
    void sendPacket(const OutputMemoryBitStream& ombs);
    void updateSayingHello();
    void handleWelcomePacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerDeniedPacket();
    void handleStatePacket(InputMemoryBitStream& imbs);
    void readLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& imbs);
    void updateSendingInputPacket();
    void sendInputPacket();
    void updateAddLocalPlayerRequest();
    void updateDropLocalPlayerRequest();
    void updateNextIndex();
    
    NetworkManagerClient(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf);
    ~NetworkManagerClient();
    NetworkManagerClient(const NetworkManagerClient&);
    NetworkManagerClient& operator=(const NetworkManagerClient&);
};
