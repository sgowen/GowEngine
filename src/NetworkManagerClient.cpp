//
//  NetworkManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkManagerClient.hpp"

#include "SocketClientHelper.hpp"
#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "DeliveryNotificationManager.hpp"
#include "MachineAddress.hpp"
#include "Entity.hpp"
#include "MoveList.hpp"
#include "ReplicationManagerClient.hpp"
#include "WeightedTimedMovingAverage.hpp"
#include "SocketAddress.hpp"
#include "Timing.hpp"

#include "StringUtil.hpp"
#include "SocketAddressFactory.hpp"
#include "Macros.hpp"
#include "InstanceManager.hpp"

#include <assert.h>

NetworkManagerClient* NetworkManagerClient::s_instance = NULL;

void NetworkManagerClient::create(ClientHelper* clientHelper, HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc, RemoveProcessedMovesFunc removeProcessedMovesFunc, GetMoveListFunc getMoveListFunc, OnPlayerWelcomedFunc onPlayerWelcomedFunc)
{
    assert(!s_instance);
    
    s_instance = new NetworkManagerClient(clientHelper, handleEntityCreatedFunc, handleEntityDeletionFunc, removeProcessedMovesFunc, getMoveListFunc, onPlayerWelcomedFunc);
}

NetworkManagerClient * NetworkManagerClient::getInstance()
{
    return s_instance;
}

void NetworkManagerClient::destroy()
{
    assert(s_instance);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkManagerClient::sProcessPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    NW_MGR_CLIENT->processPacket(inputStream, static_cast<SocketAddress*>(fromAddress));
}

void NetworkManagerClient::sHandleNoResponse()
{
    NW_MGR_CLIENT->handleNoResponse();
}

void NetworkManagerClient::sHandleConnectionReset(MachineAddress* fromAddress)
{
    NW_MGR_CLIENT->handleConnectionReset(static_cast<SocketAddress*>(fromAddress));
}

void NetworkManagerClient::processIncomingPackets()
{
    _clientHelper->processIncomingPackets();
}

void NetworkManagerClient::sendOutgoingPackets()
{
    _hasReceivedNewState = false;
    
    switch (_state)
    {
        case NCS_SayingHello:
            updateSayingHello();
            break;
        case NCS_Welcomed:
            updateSendingInputPacket();
            updateAddLocalPlayerRequest();
            updateDropLocalPlayerRequest();
            break;
        case NCS_Uninitialized:
            _clientHelper->handleUninitialized();
            break;
        case NCS_Disconnected:
            break;
    }
    
    if (_state != NCS_Disconnected)
    {
        int clientHelperState = _clientHelper->state();
        if (clientHelperState == CLIENT_READY_TO_SAY_HELLO
            && _state != NCS_Welcomed)
        {
            _state = NCS_SayingHello;
        }
        else if (clientHelperState == CLIENT_AUTH_FAILED)
        {
            _state = NCS_Disconnected;
        }
    }
}

void NetworkManagerClient::requestToAddLocalPlayer()
{
    _isRequestingToAddLocalPlayer = true;
    
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkManagerClient::requestToDropLocalPlayer(uint8_t index)
{
    if (index < 1)
    {
        return;
    }
    
    _isRequestingToDropLocalPlayer = index;
    
    _isRequestingToAddLocalPlayer = false;
    
    _indexToPlayerIDMap.erase(index);
    
    updateNextIndex();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesReceivedPerSecond() const
{
    return _clientHelper->getBytesReceivedPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesSentPerSecond() const
{
    return _clientHelper->getBytesSentPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getAvgRoundTripTime() const
{
    return *_avgRoundTripTime;
}

float NetworkManagerClient::getRoundTripTime() const
{
    return _avgRoundTripTime->getValue();
}

bool NetworkManagerClient::isPlayerIDLocal(uint8_t playerID) const
{
    for (auto const& entry : _indexToPlayerIDMap)
    {
        if (entry.second == playerID)
        {
            return true;
        }
    }
    
    return false;
}

bool NetworkManagerClient::hasReceivedNewState()
{
    return _hasReceivedNewState;
}

std::map<uint8_t, uint8_t>& NetworkManagerClient::getPlayerIDs()
{
    return _indexToPlayerIDMap;
}

std::string& NetworkManagerClient::getPlayerName()
{
    return _clientHelper->getName();
}

NetworkClientState NetworkManagerClient::state() const
{
    return _state;
}

uint32_t NetworkManagerClient::getMap()
{
    return _map;
}

EntityManager* NetworkManagerClient::getEntityManager()
{
    return _entityManager;
}

void NetworkManagerClient::processPacket(InputMemoryBitStream& inputStream, MachineAddress* fromAddress)
{
    _lastServerCommunicationTimestamp = _timing->getTime();
    
    uint8_t packetType;
    inputStream.read(packetType);
    
    switch (packetType)
    {
        case NetworkPacketType_WELCOME:
            handleWelcomePacket(inputStream);
            break;
        case NetworkPacketType_LOCAL_PLAYER_ADDED:
            handleLocalPlayerAddedPacket(inputStream);
            break;
        case NetworkPacketType_LOCAL_PLAYER_DENIED:
            handleLocalPlayerDeniedPacket();
            break;
        case NetworkPacketType_STATE:
            if (_deliveryNotificationManager->readAndProcessState(inputStream))
            {
                handleStatePacket(inputStream);
            }
            break;
        default:
            _clientHelper->processSpecialPacket(packetType, inputStream, fromAddress);
            break;
    }
}

void NetworkManagerClient::handleNoResponse()
{
    float time = _timing->getTime();
    
    float timeout = _state == NCS_Uninitialized ? NW_CONNECT_TO_SERVER_TIMEOUT : NW_SERVER_TIMEOUT;
    if (time > _lastServerCommunicationTimestamp + timeout)
    {
        _state = NCS_Disconnected;
    }
}

void NetworkManagerClient::handleConnectionReset(MachineAddress* fromAddress)
{
    UNUSED(fromAddress);
}

void NetworkManagerClient::sendPacket(const OutputMemoryBitStream& outputStream)
{
    _clientHelper->sendPacket(outputStream);
}

void NetworkManagerClient::updateSayingHello()
{
    float time = _timing->getTime();
    
    if (time > _timeOfLastHello + NW_CLIENT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream helloPacket;
        
        helloPacket.write(static_cast<uint8_t>(NetworkPacketType_HELLO));
        helloPacket.writeSmall(getPlayerName());
        
        sendPacket(helloPacket);
        
        _timeOfLastHello = time;
    }
}

void NetworkManagerClient::handleWelcomePacket(InputMemoryBitStream& inputStream)
{
    if (_state == NCS_SayingHello)
    {
        // if we got a player id, we've been welcomed!
        uint8_t playerID;
        inputStream.read<uint8_t, 3>(playerID);
        
        _state = NCS_Welcomed;
        
        _indexToPlayerIDMap.clear();
        _indexToPlayerIDMap[_nextIndex] = playerID;
        
        LOG("'%s' was welcomed on client as player %d", getPlayerName().c_str(), playerID);
        
        _onPlayerWelcomedFunc(playerID);
        
        updateNextIndex();
    }
}

void NetworkManagerClient::handleLocalPlayerAddedPacket(InputMemoryBitStream& inputStream)
{
    if (_state == NCS_Welcomed
        && _isRequestingToAddLocalPlayer)
    {
        // if we got a player id, our local player has been added!
        uint8_t playerID;
        inputStream.read(playerID);
        
        _indexToPlayerIDMap[_nextIndex] = playerID;
        
        LOG("'%s(%d)' was welcomed on client as player %d", getPlayerName().c_str(), _nextIndex, playerID);
        
        _isRequestingToAddLocalPlayer = false;
        
        _onPlayerWelcomedFunc(playerID);
        
        updateNextIndex();
    }
}

void NetworkManagerClient::handleLocalPlayerDeniedPacket()
{
    LOG("'%s(%d)' was denied on client...", getPlayerName().c_str(), _nextIndex);
    
    _isRequestingToAddLocalPlayer = false;
}

void NetworkManagerClient::handleStatePacket(InputMemoryBitStream& inputStream)
{
    if (_state == NCS_Welcomed)
    {
        readLastMoveProcessedOnServerTimestamp(inputStream);
        
        //tell the replication manager to handle the rest...
        _replicationManagerClient->read(inputStream);
        
        _hasReceivedNewState = true;
    }
}

void NetworkManagerClient::readLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& inputStream)
{
    bool isTimestampDirty;
    inputStream.read(isTimestampDirty);
    if (isTimestampDirty)
    {
        inputStream.read(_lastMoveProcessedByServerTimestamp);
        
        float rtt = _timing->getTime() - _lastMoveProcessedByServerTimestamp;
        _avgRoundTripTime->update(rtt);
        
        inputStream.read(_map);
        
        _removeProcessedMovesFunc(_lastMoveProcessedByServerTimestamp);
    }
}

void NetworkManagerClient::updateSendingInputPacket()
{
    sendInputPacket();
}

void NetworkManagerClient::sendInputPacket()
{
    //only send if there's any input to sent!
    MoveList& moveList = _getMoveListFunc();
    
    if (moveList.hasMoves())
    {
        OutputMemoryBitStream inputPacket;
        
        inputPacket.write(static_cast<uint8_t>(NetworkPacketType_INPUT));
        
        _deliveryNotificationManager->writeState(inputPacket);
        
        // eventually write the 3 latest moves so they have 3 chances to get through...
        int moveCount = moveList.getNumMovesAfterTimestamp(_lastMoveProcessedByServerTimestamp);
        int firstMoveIndex = moveCount - 3;
        if (firstMoveIndex < 0)
        {
            firstMoveIndex = 0;
        }
        
        std::deque<Move>::const_iterator move = moveList.begin() + firstMoveIndex;
        
        // only need 2 bits to write the move count, because it's 0-3
        inputPacket.write<uint8_t, 2>(moveCount - firstMoveIndex);
        
        const Move* referenceMove = NULL;
        for (; firstMoveIndex < moveCount; ++firstMoveIndex, ++move)
        {
            bool needsToWriteMove = true;
            
            if (referenceMove)
            {
                if (move->isEqual(referenceMove))
                {
                    inputPacket.write(true);
                    
                    inputPacket.write(move->getTimestamp());
                    
                    needsToWriteMove = false;
                }
            }
            
            if (needsToWriteMove)
            {
                inputPacket.write(false);
                
                move->write(inputPacket);
                
                referenceMove = &(*move);
            }
        }
        
        sendPacket(inputPacket);
    }
}

void NetworkManagerClient::updateAddLocalPlayerRequest()
{
    if (_isRequestingToAddLocalPlayer)
    {
        _isRequestingToDropLocalPlayer = 0;
        
        float time = _timing->getTime();
        
        if (time > _timeOfLastHello + NW_CLIENT_TIME_BETWEEN_HELLOS)
        {
            OutputMemoryBitStream packet;
            
            packet.write(static_cast<uint8_t>(NetworkPacketType_ADD_LOCAL_PLAYER));
            packet.write(_nextIndex);
            
            sendPacket(packet);
            
            _timeOfLastHello = time;
        }
    }
}

void NetworkManagerClient::updateDropLocalPlayerRequest()
{
    if (_isRequestingToDropLocalPlayer > 0)
    {
        _isRequestingToAddLocalPlayer = false;
        
        OutputMemoryBitStream packet;
        
        packet.write(static_cast<uint8_t>(NetworkPacketType_DROP_LOCAL_PLAYER));
        packet.write(_isRequestingToDropLocalPlayer);
        
        sendPacket(packet);
        
        _isRequestingToDropLocalPlayer = 0;
    }
}

void NetworkManagerClient::updateNextIndex()
{
    // Find the next available Player ID
    _nextIndex = 0;
    for (auto const& entry : _indexToPlayerIDMap)
    {
        if (entry.first == _nextIndex)
        {
            ++_nextIndex;
        }
    }
}

NetworkManagerClient::NetworkManagerClient(ClientHelper* clientHelper, HandleEntityCreatedFunc handleEntityCreatedFunc, HandleEntityDeletionFunc handleEntityDeletionFunc, RemoveProcessedMovesFunc removeProcessedMovesFunc, GetMoveListFunc getMoveListFunc, OnPlayerWelcomedFunc onPlayerWelcomedFunc) :
_timing(static_cast<Timing*>(INSTANCE_MGR.get(InstanceKey_TIMING_CLIENT))),
_clientHelper(clientHelper),
_removeProcessedMovesFunc(removeProcessedMovesFunc),
_getMoveListFunc(getMoveListFunc),
_onPlayerWelcomedFunc(onPlayerWelcomedFunc),
_entityManager(new EntityManager(handleEntityCreatedFunc, handleEntityDeletionFunc)),
_replicationManagerClient(new ReplicationManagerClient(_entityManager)),
_avgRoundTripTime(new WeightedTimedMovingAverage(_timing, 1.f)),
_state(NCS_Uninitialized),
_deliveryNotificationManager(new DeliveryNotificationManager(_timing, true, false)),
_timeOfLastHello(0.0f),
_lastMoveProcessedByServerTimestamp(0.0f),
_lastServerCommunicationTimestamp(0),
_isRequestingToAddLocalPlayer(false),
_isRequestingToDropLocalPlayer(0),
_nextIndex(0),
_hasReceivedNewState(false)
{
    // Empty
}

NetworkManagerClient::~NetworkManagerClient()
{
    delete _clientHelper;
    delete _entityManager;
    delete _replicationManagerClient;
    delete _avgRoundTripTime;
    delete _deliveryNotificationManager;
}
