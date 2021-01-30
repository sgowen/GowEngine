//
//  ClientProxy.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ClientProxy.hpp"

#include "EntityManager.hpp"
#include "ReplicationManagerServer.hpp"
#include "MachineAddress.hpp"
#include "Timing.hpp"

#include "InstanceManager.hpp"

ClientProxy::ClientProxy(EntityManager* entityManager, MachineAddress* machineAddress, const std::string& name, uint8_t playerID) :
_timing(static_cast<Timing*>(INSTANCE_MGR.get(INSK_TIMING_SERVER))),
_deliveryNotificationManager(DeliveryNotificationManager(_timing, false, true)),
_replicationManagerServer(new ReplicationManagerServer(entityManager)),
_machineAddress(machineAddress->createNewCopy()),
_name(name),
_lastPacketFromClientTime(0),
_isLastMoveTimestampDirty(false)
{
    _playerIDs.push_back(playerID);
    
    updateLastPacketTime();
}

ClientProxy::~ClientProxy()
{
    delete _machineAddress;
}

MachineAddress* ClientProxy::getMachineAddress() const
{
    return _machineAddress;
}

uint8_t ClientProxy::getPlayerID(uint8_t index) const
{
    return _playerIDs.size() > index ? _playerIDs[index] : NW_INPUT_UNASSIGNED;
}

const std::string& ClientProxy::getName() const
{
    return _name;
}

void ClientProxy::updateLastPacketTime()
{
    _lastPacketFromClientTime = _timing->getTime();
}

float ClientProxy::getLastPacketFromClientTime() const
{
    return _lastPacketFromClientTime;
}

DeliveryNotificationManager& ClientProxy::getDeliveryNotificationManager()
{
    return _deliveryNotificationManager;
}

ReplicationManagerServer* ClientProxy::getReplicationManagerServer()
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
