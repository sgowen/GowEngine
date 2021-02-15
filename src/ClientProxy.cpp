//
//  ClientProxy.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ClientProxy.hpp"

#include "EntityRegistry.hpp"
#include "SocketAddress.hpp"
#include "TimeTracker.hpp"
#include "InstanceRegistry.hpp"

ClientProxy::ClientProxy(EntityRegistry& entityRegistry, SocketAddress* socketAddress, std::string username, uint8_t playerID) :
_deliveryNotificationManager(INST_REG.get<TimeTracker>(INSK_TIME_SRVR), false, true),
_replicationManagerServer(entityRegistry),
_socketAddress(socketAddress->createNewCopy()),
_username(username),
_lastPacketFromClientTime(0),
_isLastMoveTimestampDirty(false)
{
    _playerIDs.push_back(playerID);
    
    updateLastPacketTime();
}

ClientProxy::~ClientProxy()
{
    delete _socketAddress;
}

SocketAddress* ClientProxy::getSocketAddress() const
{
    return _socketAddress;
}

uint8_t ClientProxy::getPlayerID(uint8_t index) const
{
    return _playerIDs.size() > index ? _playerIDs[index] : NW_INPUT_UNASSIGNED;
}

const std::string& ClientProxy::getUsername() const
{
    return _username;
}

void ClientProxy::updateLastPacketTime()
{
    _lastPacketFromClientTime = INST_REG.get<TimeTracker>(INSK_TIME_SRVR)->_time;
}

uint32_t ClientProxy::getLastPacketFromClientTime() const
{
    return _lastPacketFromClientTime;
}

DeliveryNotificationManager& ClientProxy::getDeliveryNotificationManager()
{
    return _deliveryNotificationManager;
}

ReplicationManagerServer& ClientProxy::getReplicationManagerServer()
{
    return _replicationManagerServer;
}

MoveList& ClientProxy::getUnprocessedMoveList()
{
    return _unprocessedMoveList;
}

void ClientProxy::setLastMoveTimestampDirty(bool isDirty)
{
    _isLastMoveTimestampDirty = isDirty;
}

bool ClientProxy::isLastMoveTimestampDirty() const
{
    return _isLastMoveTimestampDirty;
}

void ClientProxy::onLocalPlayerAdded(uint8_t playerID)
{
    for (typename std::vector<uint8_t>::iterator i = _playerIDs.begin(); i != _playerIDs.end(); )
    {
        if ((*i) == playerID)
        {
            return;
        }
        else
        {
            ++i;
        }
    }
    
    _playerIDs.push_back(playerID);
}

void ClientProxy::onLocalPlayerRemoved(uint8_t playerID)
{
    for (typename std::vector<uint8_t>::iterator i = _playerIDs.begin(); i != _playerIDs.end(); )
    {
        if ((*i) == playerID)
        {
            i = _playerIDs.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

std::vector<uint8_t>& ClientProxy::getPlayerIDs()
{
    return _playerIDs;
}

uint8_t ClientProxy::getNumPlayers()
{
    return static_cast<uint8_t>(_playerIDs.size());
}
