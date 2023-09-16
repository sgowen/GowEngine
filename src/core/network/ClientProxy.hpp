//
//  ClientProxy.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "DeliveryNotificationManager.hpp"
#include "ReplicationManagerServer.hpp"
#include "MoveList.hpp"

#include <string>
#include <vector>

class EntityRegistry;
class SocketAddress;

class ClientProxy
{
public:
    ClientProxy(EntityRegistry& er, TimeTracker& tt, SocketAddress* sa, std::string username, uint8_t playerID);
    ~ClientProxy();
    
    SocketAddress* getSocketAddress() const;
    uint8_t getPlayerID(uint8_t index = 0) const;
    const std::string& getUsername() const;
    void updateLastPacketTime();
    uint32_t getLastPacketFromClientTime()	const;
    DeliveryNotificationManager& getDeliveryNotificationManager();
    ReplicationManagerServer& replicationManagerServer();
    MoveList& getUnprocessedMoveList();
    void setLastMoveTimestampDirty(bool isDirty);
    bool isLastMoveTimestampDirty() const;
    void onLocalPlayerAdded(uint8_t playerID);
    void onLocalPlayerRemoved(uint8_t playerID);
    std::vector<uint8_t>& getPlayerIDs();
    uint8_t getNumPlayers();
    
private:
    TimeTracker& _timeTracker;
    DeliveryNotificationManager	_deliveryNotificationManager;
    ReplicationManagerServer _replicationManagerServer;
    SocketAddress* _socketAddress;
    std::string _username;
    std::vector<uint8_t> _playerIDs;
    uint32_t _lastPacketFromClientTime;
    MoveList _unprocessedMoveList;
    bool _isLastMoveTimestampDirty;
};
