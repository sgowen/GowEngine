//
//  NetworkClient.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/entity/EntityRegistry.hpp"

#include "DeliveryNotificationManager.hpp"
#include "ReplicationManagerClient.hpp"
#include "MovingAverage.hpp"

#include <map>
#include <string>
#include <vector>

struct TimeTracker;
class InputMemoryBitStream;
class OutputMemoryBitStream;
class SocketAddress;
class Entity;
class MoveList;

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
    static void create(ClientHelper* clientHelper, TimeTracker& tt, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, OnPlayerWelcomedFunc opwf);
    static NetworkClient* getInstance();
    static void destroy();
    
    static void sProcessPacket(InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress);
    
    NetworkClientState processIncomingPackets();
    void sendOutgoingPackets(MoveList& ml);
    void requestToAddLocalPlayer();
    void requestToDropLocalPlayer(uint8_t index);
    float bytesReceivedPerSecond() const;
    float bytesSentPerSecond() const;
    float avgRoundTripTime() const;
    bool isPlayerIDLocal(uint8_t playerID) const;
    bool hasReceivedNewState();
    std::map<uint8_t, uint8_t>& getPlayerIDs();
    std::string& getPlayerName();
    NetworkClientState state() const;
    bool connect();
    void processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress);
    uint32_t getNumMovesProcessed();
    
private:
    static NetworkClient* s_instance;
    
    TimeTracker& _timeTracker;
    ClientHelper* _clientHelper;
    OnPlayerWelcomedFunc _onPlayerWelcomedFunc;
    DeliveryNotificationManager _deliveryNotificationManager;
    EntityRegistry _entityRegistry;
    ReplicationManagerClient _replicationManagerClient;
    NetworkClientState _state;
    uint32_t _timeOfLastHello;
    std::map<uint8_t, uint8_t> _playerIDs;
    uint8_t _nextIndex;
    uint32_t _lastMoveProcessedByServerTimestamp;
    uint32_t _lastServerCommunicationTimestamp;
    MovingAverage _avgRoundTripTime;
    bool _isRequestingToAddLocalPlayer;
    uint8_t _isRequestingToDropLocalPlayer;
    bool _hasReceivedNewState;
    uint32_t _numMovesProcessed;
    
    void sendPacketToServer(const OutputMemoryBitStream& ombs);
    void updateSayingHello();
    void handleWelcomePacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs);
    void handleLocalPlayerDeniedPacket();
    void handleStatePacket(InputMemoryBitStream& imbs);
    void updateSendingInputPacket(MoveList& ml);
    void updateAddLocalPlayerRequest();
    void updateDropLocalPlayerRequest();
    void updateNextIndex();
    
    NetworkClient(ClientHelper* clientHelper, 
                  TimeTracker& tt,
                  OnEntityRegisteredFunc oerf,
                  OnEntityDeregisteredFunc oedf,
                  OnPlayerWelcomedFunc opwf);
    ~NetworkClient();
    NetworkClient(const NetworkClient&);
    NetworkClient& operator=(const NetworkClient&);
};
