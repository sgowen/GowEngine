//
//  NetworkManagerClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "EntityManager.hpp"

#include <map>
#include <string>
#include <vector>

class ClientHelper;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class DeliveryNotificationManager;
class MachineAddress;
class Entity;
class MoveList;
class ReplicationManagerClient;
class WeightedTimedMovingAverage;
class SocketAddress;
class Timing;

#define NW_MGR_CLIENT (NetworkManagerClient::getInstance())

#define NW_MGR_CLIENT_CALLBACKS NetworkManagerClient::sProcessPacket, NetworkManagerClient::sHandleNoResponse, NetworkManagerClient::sHandleConnectionReset

typedef void (*RemoveProcessedMovesFunc)(float lastMoveProcessedByServerTimestamp);
typedef MoveList& (*GetMoveListFunc)();
typedef void (*OnPlayerWelcomedFunc)(uint8_t playerID);

enum NetworkClientState
{
    NCS_Uninitialized,
    NCS_SayingHello,
    NCS_Welcomed,
    NCS_Disconnected
};

class NetworkManagerClient
{
public:
    static void create(ClientHelper* clientHelper, HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc, RemoveProcessedMovesFunc removeProcessedMovesFunc, GetMoveListFunc getMoveListFunc, OnPlayerWelcomedFunc onPlayerWelcomedFunc);
    static NetworkManagerClient* getInstance();
    static void destroy();
    
    static void sProcessPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    static void sHandleNoResponse();
    static void sHandleConnectionReset(MachineAddress* fromAddress);
    
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
    uint32_t getMap();
    EntityManager* getEntityManager();
    
private:
    static NetworkManagerClient* s_instance;
    
    Timing* _timing;
    ClientHelper* _clientHelper;
    RemoveProcessedMovesFunc _removeProcessedMovesFunc;
    GetMoveListFunc _getMoveListFunc;
    OnPlayerWelcomedFunc _onPlayerWelcomedFunc;
    DeliveryNotificationManager* _deliveryNotificationManager;
    EntityManager* _entityManager;
    ReplicationManagerClient* _replicationManagerClient;
    NetworkClientState _state;
    float _timeOfLastHello;
    std::map<uint8_t, uint8_t> _indexToPlayerIDMap;
    uint8_t _nextIndex;
    float _frameRate;
    float _lastMoveProcessedByServerTimestamp;
    float _lastServerCommunicationTimestamp;
    WeightedTimedMovingAverage* _avgRoundTripTime;
    bool _isRequestingToAddLocalPlayer;
    uint8_t _isRequestingToDropLocalPlayer;
    bool _hasReceivedNewState;
    uint32_t _map;
    
    void processPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress);
    void handleNoResponse();
    void handleConnectionReset(MachineAddress* fromAddress);
    void sendPacket(const OutputMemoryBitStream& outputStream);
    void updateSayingHello();
    void handleWelcomePacket(InputMemoryBitStream& inputStream);
    void handleLocalPlayerAddedPacket(InputMemoryBitStream& inputStream);
    void handleLocalPlayerDeniedPacket();
    void handleStatePacket(InputMemoryBitStream& inputStream);
    void readLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& inputStream);
    void updateSendingInputPacket();
    void sendInputPacket();
    void updateAddLocalPlayerRequest();
    void updateDropLocalPlayerRequest();
    void updateNextIndex();
    
    NetworkManagerClient(ClientHelper* clientHelper, HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc, RemoveProcessedMovesFunc removeProcessedMovesFunc, GetMoveListFunc getMoveListFunc, OnPlayerWelcomedFunc onPlayerWelcomedFunc);
    ~NetworkManagerClient();
    NetworkManagerClient(const NetworkManagerClient&);
    NetworkManagerClient& operator=(const NetworkManagerClient&);
};
