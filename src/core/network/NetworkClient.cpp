//
//  NetworkClient.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

NetworkClient* NetworkClient::s_instance = nullptr;

void NetworkClient::create(ClientHelper* clientHelper,
                           TimeTracker& tt,
                           OnEntityRegisteredFunc oerf,
                           OnEntityDeregisteredFunc oedf,
                           OnPlayerWelcomedFunc opwf)
{
    assert(s_instance == nullptr);
    
    s_instance = new NetworkClient(clientHelper, tt, oerf, oedf, opwf);
    
    assert(NW_CLNT != nullptr);
}

NetworkClient* NetworkClient::getInstance()
{
    return s_instance;
}

void NetworkClient::destroy()
{
    assert(s_instance != nullptr);
    
    delete s_instance;
    s_instance = nullptr;
}

void NetworkClient::sProcessPacket(InputMemoryBitStream& inInputStream, MachineAddress* inFromAddress)
{
    NW_CLNT->processPacket(inInputStream, static_cast<SocketAddress*>(inFromAddress));
}

NetworkClientState NetworkClient::processIncomingPackets()
{
    _hasReceivedNewState = false;
    
    _clientHelper->processIncomingPackets();
    
    uint32_t time = _timeTracker._time;
    uint32_t timeout = ENGINE_CFG.framesPerSecond() * 10;
    uint32_t dcTime = _lastServerCommunicationTimestamp + timeout;
    if (time > dcTime)
    {
        _state = NWCS_DISCONNECTED;
    }
    
    return _state;
}

void NetworkClient::sendOutgoingPackets(MoveList& ml)
{
    switch (_state)
    {
        case NWCS_SAYING_HELLO:
            updateSayingHello();
            break;
        case NWCS_WELCOMED:
            updateSendingInputPacket(ml);
            updateAddLocalPlayerRequest();
            updateDropLocalPlayerRequest();
            break;
        case NWCS_DISCONNECTED:
        default:
            break;
    }
    
    if (_state != NWCS_DISCONNECTED)
    {
        int clientHelperState = _clientHelper->getState();
        if (clientHelperState == CLIENT_READY_TO_SAY_HELLO
            && _state != NWCS_WELCOMED)
        {
            _state = NWCS_SAYING_HELLO;
        }
        else if (clientHelperState == CLIENT_AUTH_FAILED)
        {
            // TODO, get Steam P2P auth working
//            _state = NWCS_DISCONNECTED;
            _state = NWCS_WELCOMED;
        }
    }
}

void NetworkClient::requestToAddLocalPlayer()
{
    _isRequestingToAddLocalPlayer = true;
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkClient::requestToDropLocalPlayer(uint8_t index)
{
    assert(index >= 1);
    
    _isRequestingToAddLocalPlayer = false;
    _isRequestingToDropLocalPlayer = index;
    _playerIDs.erase(index);
    updateNextIndex();
}

float NetworkClient::bytesReceivedPerSecond() const
{
    return _clientHelper->bytesReceivedPerSecond().value();
}

float NetworkClient::bytesSentPerSecond() const
{
    return _clientHelper->bytesSentPerSecond().value();
}

float NetworkClient::avgRoundTripTime() const
{
    return _avgRoundTripTime.value() * 1000.0f;
}

bool NetworkClient::isPlayerIDLocal(uint8_t playerID) const
{
    for (auto const& entry : _playerIDs)
    {
        if (entry.second == playerID)
        {
            return true;
        }
    }
    
    return false;
}

bool NetworkClient::hasReceivedNewState()
{
    return _hasReceivedNewState;
}

std::map<uint8_t, uint8_t>& NetworkClient::getPlayerIDs()
{
    return _playerIDs;
}

std::string& NetworkClient::getPlayerName()
{
    return _clientHelper->getName();
}

NetworkClientState NetworkClient::state() const
{
    return _state;
}

bool NetworkClient::connect()
{
    int error = _clientHelper->connect();
    
    if (error != NO_ERROR &&
        ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Client connect failed. Error code %d", error);
    }
    
    return error == NO_ERROR;
}

void NetworkClient::processPacket(InputMemoryBitStream& imbs, SocketAddress* fromAddress)
{
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Client processPacket bit length: %d", imbs.getRemainingBitCount());
    }
    
    _lastServerCommunicationTimestamp = _timeTracker._time;
    
    uint8_t packetType;
    imbs.readBits(packetType, 4);
    
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
            handleStatePacket(imbs);
            break;
        case NWPT_SRVR_EXIT:
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("Server has shut down");
            }
            _state = NWCS_DISCONNECTED;
            break;
        default:
            if (ENGINE_CFG.networkLoggingEnabled())
            {
                LOG("Unknown packet type received from %s", fromAddress->toString().c_str());
            }
            break;
    }
}

uint32_t NetworkClient::getNumMovesProcessed()
{
    return _numMovesProcessed;
}

void NetworkClient::sendPacketToServer(const OutputMemoryBitStream& ombs)
{
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Client    sendPacket bit length: %d", ombs.getBitLength());
    }
    
    _clientHelper->sendPacketToServer(ombs);
}

void NetworkClient::updateSayingHello()
{
    uint32_t time = _timeTracker._time;
    uint32_t timeBetweenHellos = ENGINE_CFG.framesPerSecond();
    if (time > _timeOfLastHello + timeBetweenHellos)
    {
        // FIXME, player name length (and this packet size)
        // should be configurable
        OutputMemoryBitStream ombs(64);
        ombs.writeBits(static_cast<uint8_t>(NWPT_HELLO), 4);
        ombs.writeSmall(getPlayerName());
        sendPacketToServer(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkClient::handleWelcomePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_SAYING_HELLO)
    {
        return;
    }
    
    uint8_t playerID;
    imbs.readBits(playerID, 3);
    
    _state = NWCS_WELCOMED;
    
    _playerIDs.clear();
    _playerIDs[_nextIndex] = playerID;
    
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("Client '%s' was welcomed as player %d", getPlayerName().c_str(), playerID);
    }
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkClient::handleLocalPlayerAddedPacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED || !_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    uint8_t playerID;
    imbs.read(playerID);
    
    _playerIDs[_nextIndex] = playerID;
    
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("'%s(%d)' was welcomed on client as player %d", getPlayerName().c_str(), _nextIndex, playerID);
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    _onPlayerWelcomedFunc(playerID);
    
    updateNextIndex();
}

void NetworkClient::handleLocalPlayerDeniedPacket()
{
    if (ENGINE_CFG.networkLoggingEnabled())
    {
        LOG("'%s(%d)' was denied on client...", getPlayerName().c_str(), _nextIndex);
    }
    
    _isRequestingToAddLocalPlayer = false;
}

void NetworkClient::handleStatePacket(InputMemoryBitStream& imbs)
{
    if (_state != NWCS_WELCOMED)
    {
        return;
    }
    
    if (!_deliveryNotificationManager.readAndProcessState(imbs))
    {
        return;
    }
    
    imbs.read(_numMovesProcessed);
    
    bool isTimestampDirty;
    imbs.read(isTimestampDirty);
    if (isTimestampDirty)
    {
        imbs.read(_lastMoveProcessedByServerTimestamp);
        
        float rtt = _timeTracker.realTime() - _timeTracker.realTime(_lastMoveProcessedByServerTimestamp);
        _avgRoundTripTime.update(rtt);
    }
    
    _replicationManagerClient.read(imbs, _entityRegistry);
    _hasReceivedNewState = true;
}

void NetworkClient::updateSendingInputPacket(MoveList& ml)
{
    OutputMemoryBitStream ombs(NW_MAX_PACKET_SIZE);
    ombs.writeBits(static_cast<uint8_t>(NWPT_INPUT), 4);
    
    _deliveryNotificationManager.writeState(ombs);
    
    ombs.write(ml.hasMoves());
    if (ml.hasMoves())
    {
        static uint8_t maxNumFramesOfRollback = ENGINE_CFG.maxNumFramesOfRollback();
        static uint8_t numFramesOfInputDelay = ENGINE_CFG.numFramesOfInputDelay();
        static uint8_t maxNumMoves = numFramesOfInputDelay + maxNumFramesOfRollback + 1;
        
        uint8_t moveCount = ml.getNumMovesAfterTimestamp(_lastMoveProcessedByServerTimestamp);
        assert(moveCount <= maxNumMoves);
        ombs.writeBits(moveCount, NBITS(maxNumMoves));
        
        // TODO, check this.
        // shouldn't the last X moves after _lastMoveProcessedByServerTimestamp
        // be from the end? #tyingCommentsTogetherForTheFirstTime
        std::deque<Move>::const_iterator moveItr = ml.begin();
        
        uint8_t moveCountWritten = 0;
        for (int i = 0; i < ml.getMoveCount(); ++i, ++moveItr)
        {
            // Maybe this solves my above complaint?
            // #tyingCommentsTogetherForTheFirstTime
            if (moveItr->getTimestamp() > _lastMoveProcessedByServerTimestamp)
            {
                moveItr->write(ombs);
                ++moveCountWritten;
            }
        }
        
        assert(moveCountWritten == moveCount);
    }
    
    sendPacketToServer(ombs);
}

void NetworkClient::updateAddLocalPlayerRequest()
{
    if (!_isRequestingToAddLocalPlayer)
    {
        return;
    }
    
    uint32_t time = _timeTracker._time;
    uint32_t timeBetweenHellos = ENGINE_CFG.framesPerSecond();
    if (time > _timeOfLastHello + timeBetweenHellos)
    {
        OutputMemoryBitStream ombs(2);
        ombs.writeBits(static_cast<uint8_t>(NWPT_ADD_LOCAL_PLAYER), 4);
        ombs.write(_nextIndex);
        sendPacketToServer(ombs);
        
        _timeOfLastHello = time;
    }
}

void NetworkClient::updateDropLocalPlayerRequest()
{
    if (_isRequestingToDropLocalPlayer == 0)
    {
        return;
    }
    
    _isRequestingToAddLocalPlayer = false;
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_DROP_LOCAL_PLAYER), 4);
    ombs.write(_isRequestingToDropLocalPlayer);
    sendPacketToServer(ombs);
    
    _isRequestingToDropLocalPlayer = 0;
}

void NetworkClient::updateNextIndex()
{
    _nextIndex = 0;
    for (auto const& entry : _playerIDs)
    {
        if (entry.first == _nextIndex)
        {
            ++_nextIndex;
        }
    }
}

NetworkClient::NetworkClient(ClientHelper* clientHelper,
                             TimeTracker& tt,
                             OnEntityRegisteredFunc oerf,
                             OnEntityDeregisteredFunc oedf,
                             OnPlayerWelcomedFunc opwf) :
_timeTracker(tt),
_clientHelper(clientHelper),
_onPlayerWelcomedFunc(opwf),
_deliveryNotificationManager(_timeTracker, true, false),
_entityRegistry(oerf, oedf),
_replicationManagerClient(),
_state(NWCS_SAYING_HELLO),
_timeOfLastHello(0),
_nextIndex(0),
_lastMoveProcessedByServerTimestamp(0),
_lastServerCommunicationTimestamp(0),
_avgRoundTripTime(_timeTracker, 1.0f),
_isRequestingToAddLocalPlayer(false),
_isRequestingToDropLocalPlayer(0),
_hasReceivedNewState(false),
_numMovesProcessed(0)
{
    // Empty
}

NetworkClient::~NetworkClient()
{
    _entityRegistry.deregisterAll();
    
    OutputMemoryBitStream ombs(1);
    ombs.writeBits(static_cast<uint8_t>(NWPT_CLNT_EXIT), 4);
    sendPacketToServer(ombs);
    
    _deliveryNotificationManager.logStats();
    
    delete _clientHelper;
}
