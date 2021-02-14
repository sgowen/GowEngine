//
//  NetworkManagerClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "NetworkManagerClient.hpp"

#include "InputMemoryBitStream.hpp"
#include "OutputMemoryBitStream.hpp"
#include "SocketAddress.hpp"
#include "Entity.hpp"
#include "MoveList.hpp"
#include "SocketAddress.hpp"
#include "TimeTracker.hpp"

#include "StringUtil.hpp"
#include "SocketAddressFactory.hpp"
#include "Macros.hpp"
#include "InstanceRegistry.hpp"

#include <assert.h>

NetworkManagerClient* NetworkManagerClient::s_instance = NULL;

void NetworkManagerClient::create(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf)
{
    assert(s_instance == NULL);
    
    s_instance = new NetworkManagerClient(serverIPAddress, username, port, oerf, oedf, rpmf, gmlf, opwf);
}

NetworkManagerClient * NetworkManagerClient::getInstance()
{
    return s_instance;
}

void NetworkManagerClient::destroy()
{
    assert(s_instance != NULL);
    
    delete s_instance;
    s_instance = NULL;
}

void NetworkManagerClient::processIncomingPackets()
{
    _packetHandler.processIncomingPackets();
}

void NetworkManagerClient::sendOutgoingPackets()
{
    _hasReceivedNewState = false;
    
    switch (_state)
    {
        case NWCS_SAYING_HELLO:
            updateSayingHello();
            break;
        case NWCS_WELCOMED:
            updateSendingInputPacket();
            updateAddLocalPlayerRequest();
            updateDropLocalPlayerRequest();
            break;
        case NWCS_DISCONNECTED:
        default:
            break;
    }
}

void NetworkManagerClient::requestToAddLocalPlayer()
{
    _isRequestingToAddLocalPlayer = true;
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkManagerClient::requestToDropLocalPlayer(uint8_t index)
{
    assert(index >= 1);
    
    _isRequestingToAddLocalPlayer = false;
    _isRequestingToDropLocalPlayer = index;
    _indexToPlayerIDMap.erase(index);
    updateNextIndex();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesReceivedPerSecond() const
{
    return _packetHandler.getBytesReceivedPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getBytesSentPerSecond() const
{
    return _packetHandler.getBytesSentPerSecond();
}

const WeightedTimedMovingAverage& NetworkManagerClient::getAvgRoundTripTime() const
{
    return _avgRoundTripTime;
}

float NetworkManagerClient::getRoundTripTime() const
{
    return _avgRoundTripTime.getValue();
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
    return _username;
}

NetworkClientState NetworkManagerClient::state() const
{
    return _state;
}

bool NetworkManagerClient::isConnected()
{
    return _packetHandler.isConnected();
}

EntityRegistry& NetworkManagerClient::getEntityRegistry()
{
    return _entityRegistry;
}

void NetworkManagerClient::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    _lastServerCommunicationTimestamp = tt->_time;
    
    uint8_t packetType;
    imbs.read(packetType);
    
    switch (packetType)
    {
        case NWPT_WELCOME:
            handleWelcomePacket(imbs);
            break;
        case NWPT_LOCAL_PLAYER_ADDED:
            handleLocalPlayerAddedPacket(imbs);
            break;
        case NWPT_LOCAL_PLAYER_DENIED:
            handleLocalPlayerDeniedPacket();
            break;
        case NWPT_STATE:
            if (_deliveryNotificationManager.readAndProcessState(imbs))
            {
                handleStatePacket(imbs);
            }
            break;
        case NWPT_SRVR_EXIT:
            LOG("Server has shut down");
            _state = NWCS_DISCONNECTED;
            break;
        default:
            LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
            break;
    }
}

void NetworkManagerClient::handleNoResponse()
{
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    uint32_t time = tt->_time;
    
    uint32_t timeout = NW_SRVR_TIMEOUT;
    float dcTime = _lastServerCommunicationTimestamp + timeout;
    if (time > dcTime)
    {
        _state = NWCS_DISCONNECTED;
    }
}

void NetworkManagerClient::handleConnectionReset(SocketAddress* fromAddress)
{
    UNUSED(fromAddress);
}

void NetworkManagerClient::sendPacket(const OutputMemoryBitStream& ombs)
{
    _packetHandler.sendPacket(ombs, _serverAddress);
}

void NetworkManagerClient::updateSayingHello()
{
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    float time = tt->_time;
    
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream helloPacket;
        
        helloPacket.write(static_cast<uint8_t>(NWPT_HELLO));
        helloPacket.writeSmall(getPlayerName());
        
        sendPacket(helloPacket);
        
        _timeOfLastHello = time;
    }
}

void NetworkManagerClient::handleWelcomePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_SAYING_HELLO)
    {
        return;
    }
    
    // if we got a player ID, we've been welcomed!
    uint8_t playerID;
    imbs.read<uint8_t, 3>(playerID);
    
    _state = NWCS_WELCOMED;
    
    _indexToPlayerIDMap.clear();
    _indexToPlayerIDMap[_nextIndex] = playerID;
    
    LOG("'%s' was welcomed on client as player %d", getPlayerName().c_str(), playerID);
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkManagerClient::handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED || !_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    // if we got a player ID, our local player has been added!
    uint8_t playerID;
    imbs.read(playerID);
    
    _indexToPlayerIDMap[_nextIndex] = playerID;
    
    LOG("'%s(%d)' was welcomed on client as player %d", getPlayerName().c_str(), _nextIndex, playerID);
    
    _isRequestingToAddLocalPlayer = false;
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkManagerClient::handleLocalPlayerDeniedPacket()
{
    LOG("'%s(%d)' was denied on client...", getPlayerName().c_str(), _nextIndex);
    
    _isRequestingToAddLocalPlayer = false;
}

void NetworkManagerClient::handleStatePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED)
    {
        return;
    }
    
    readLastMoveProcessedOnServerTimestamp(imbs);
    _replicationManagerClient.read(imbs);
    _hasReceivedNewState = true;
}

void NetworkManagerClient::readLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& imbs)
{
    bool isTimestampDirty;
    imbs.read(isTimestampDirty);
    if (isTimestampDirty)
    {
        imbs.read(_lastMoveProcessedByServerTimestamp);
        
        TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
        float rtt = tt->realTime() - tt->realTime(_lastMoveProcessedByServerTimestamp);
        _avgRoundTripTime.update(rtt);
        
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
        
        inputPacket.write(static_cast<uint8_t>(NWPT_INPUT));
        
        _deliveryNotificationManager.writeState(inputPacket);
        
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
            
            if (referenceMove != NULL && move->isEqual(referenceMove))
            {
                inputPacket.write(true);
                inputPacket.write(move->getTimestamp());
                
                needsToWriteMove = false;
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
    if (!_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    TimeTracker* tt = INST_REG.get<TimeTracker>(INSK_TIME_CLNT);
    float time = tt->_time;
    
    if (time > _timeOfLastHello + NW_CLNT_TIME_BETWEEN_HELLOS)
    {
        OutputMemoryBitStream packet;
        
        packet.write(static_cast<uint8_t>(NWPT_ADD_LOCAL_PLAYER));
        packet.write(_nextIndex);
        
        sendPacket(packet);
        
        _timeOfLastHello = time;
    }
}

void NetworkManagerClient::updateDropLocalPlayerRequest()
{
    if (_isRequestingToDropLocalPlayer == 0)
    {
        return;
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    OutputMemoryBitStream packet;
    
    packet.write(static_cast<uint8_t>(NWPT_DROP_LOCAL_PLAYER));
    packet.write(_isRequestingToDropLocalPlayer);
    
    sendPacket(packet);
    
    _isRequestingToDropLocalPlayer = 0;
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

void cb_client_processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    NW_MGR_CLNT->processPacket(imbs, fromAddress);
}

void cb_client_handleNoResponse()
{
    NW_MGR_CLNT->handleNoResponse();
}

void cb_client_handleConnectionReset(SocketAddress* fromAddress)
{
    NW_MGR_CLNT->handleConnectionReset(fromAddress);
}

#define PACKET_HANDLER_CBS cb_client_processPacket, cb_client_handleNoResponse, cb_client_handleConnectionReset

NetworkManagerClient::NetworkManagerClient(std::string serverIPAddress, std::string username, uint16_t port, OnEntityRegisteredFunc oerf, OnEntityDeregisteredFunc oedf, RemoveProcessedMovesFunc rpmf, GetMoveListFunc gmlf, OnPlayerWelcomedFunc opwf) :
_packetHandler(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), false, port, PACKET_HANDLER_CBS),
_serverAddress(SocketAddressFactory::createIPv4FromString(serverIPAddress)),
_username(username),
_removeProcessedMovesFunc(rpmf),
_getMoveListFunc(gmlf),
_onPlayerWelcomedFunc(opwf),
_entityRegistry(oerf, oedf),
_replicationManagerClient(_entityRegistry),
_avgRoundTripTime(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), 1.0f),
_state(NWCS_SAYING_HELLO),
_deliveryNotificationManager(INST_REG.get<TimeTracker>(INSK_TIME_CLNT), true, false),
_timeOfLastHello(0),
_lastMoveProcessedByServerTimestamp(0),
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
    OutputMemoryBitStream packet;
    packet.write(static_cast<uint8_t>(NWPT_CLNT_EXIT));
    sendPacket(packet);
}
