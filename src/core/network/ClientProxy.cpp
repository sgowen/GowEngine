//
//  ClientProxy.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ClientProxy::ClientProxy(EntityRegistry& er, TimeTracker& tt, SocketAddress* sa, std::string username, uint8_t playerID) :
_timeTracker(tt),
_deliveryNotificationManager(_timeTracker, false, true),
_replicationManagerServer(er),
_socketAddress(sa->createNewCopy()),
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
    return _playerIDs.size() > index ? _playerIDs[index] : 0;
}

const std::string& ClientProxy::getUsername() const
{
    return _username;
}

void ClientProxy::updateLastPacketTime()
{
    _lastPacketFromClientTime = _timeTracker._time;
}

uint32_t ClientProxy::getLastPacketFromClientTime() const
{
    return _lastPacketFromClientTime;
}

DeliveryNotificationManager& ClientProxy::getDeliveryNotificationManager()
{
    return _deliveryNotificationManager;
}

ReplicationManagerServer& ClientProxy::replicationManagerServer()
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
