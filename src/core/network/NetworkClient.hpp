//
//  NetworkClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityRegistry.hpp"

#include "PacketHandler.hpp"
#include "DeliveryNotificationManager.hpp"
#include "ReplicationManagerClient.hpp"
#include "MovingAverage.hpp"

#include <map>
#include <string>
#include <vector>

class InputMemoryBitStream;
class OutputMemoryBitStream;
class SocketAddress;
class Entity;
class MoveList;

#define NW_CLNT (NetworkClient::getInstance())

typedef void (*RemoveProcessedMovesFunc)(uint32_t lastMoveProcessedByServerTimestamp);
typedef void (*OnPlayerWelcomedFunc)(uint8_t playerID);

enum NetworkClientState
{
    NWCS_SAYING_HELLO,
    NWCS_WELCOMED,
    NWCS_DISCONNECTED
};

class NetworkClient
{
public:
    static void create(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, OnPlayerWelcomedFunc opwf);
    static NetworkClient* getInstance();
    static void destroy();
    
    NetworkClientState processIncomingPackets();
    void sendOutgoingPackets(MoveList& ml);
    void requestToAddLocalPlayer();
    void requestToDropLocalPlayer(uint8_t index);
    const MovingAverage& getBytesReceivedPerSecond() const;
    const MovingAverage& getBytesSentPerSecond() const;
    const MovingAverage& getAvgRoundTripTime()	const;
    float getRoundTripTime() const;
    bool isPlayerIDLocal(uint8_t playerID) const;
    bool hasReceivedNewState();
    std::map<uint8_t, uint8_t>& getPlayerIDs();
    std::string& getPlayerName();
    NetworkClientState state() const;
    bool connect();
    void processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
    void onMoveProcessed();
    uint32_t getNumMovesProcessed();
    
private:
    static NetworkClient* s_instance;
    
    PacketHandler _packetHandler;
    SocketAddress* _serverAddress;
    std::string _username;
    RemoveProcessedMovesFunc _removeProcessedMovesFunc;
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
    MovingAverage _avgRoundTripTime;
    bool _isRequestingToAddLocalPlayer;
    uint8_t _isRequestingToDropLocalPlayer;
    bool _hasReceivedNewState;
    uint32_t _numMovesProcessed;
    uint16_t _port;
    
    void sendPacket(const OutputMemoryBitStream& ombs);
    void updateSayingHello();
    void handleWelcomePacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerDeniedPacket();
    void handleStatePacket(InputMemoryBitStream& imbs);
    void updateSendingInputPacket(MoveList& ml);
    void updateAddLocalPlayerRequest();
    void updateDropLocalPlayerRequest();
    void updateNextIndex();
    
    NetworkClient(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, OnPlayerWelcomedFunc opwf);
    ~NetworkClient();
    NetworkClient(const NetworkClient&);
    NetworkClient& operator=(const NetworkClient&);
};
