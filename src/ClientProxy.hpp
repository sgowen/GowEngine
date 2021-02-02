//
//  ClientProxy.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "DeliveryNotificationManager.hpp"
#include "MoveList.hpp"

#include <string>
#include <vector>

class EntityManager;
class ReplicationManagerServer;
class MachineAddress;
class TimeTracker;

class ClientProxy
{
public:
    ClientProxy(EntityManager* entityManager, MachineAddress* machineAddress, const std::string& name, uint8_t playerID);
    ~ClientProxy();
    
    MachineAddress* getMachineAddress() const;
    uint8_t getPlayerID(uint8_t index = 0) const;
    const std::string& getName() const;
    void updateLastPacketTime();
    float getLastPacketFromClientTime()	const;
    DeliveryNotificationManager& getDeliveryNotificationManager();
    ReplicationManagerServer* getReplicationManagerServer();
    MoveList& getUnprocessedMoveList();
    void setLastMoveTimestampDirty(bool isDirty);
    bool isLastMoveTimestampDirty() const;
    void onLocalPlayerAdded(uint8_t playerID);
    void onLocalPlayerRemoved(uint8_t playerID);
    std::vector<uint8_t>& getPlayerIDs();
    uint8_t getNumPlayers();
    
private:
    TimeTracker* _timeTracker;
    DeliveryNotificationManager	_deliveryNotificationManager;
    ReplicationManagerServer* _replicationManagerServer;
    MachineAddress* _machineAddress;
    std::string _name;
    std::vector<uint8_t> _playerIDs;
    float _lastPacketFromClientTime;
    MoveList _unprocessedMoveList;
    bool _isLastMoveTimestampDirty;
};
