//
//  DeliveryNotificationManager.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "DeliveryNotificationManager.hpp"

#include "TimeTracker.hpp"
#include "OutputMemoryBitStream.hpp"
#include "InputMemoryBitStream.hpp"
#include "StringUtil.hpp"
#include "PlatformMacros.hpp"

DeliveryNotificationManager::DeliveryNotificationManager(TimeTracker* tt, bool shouldSendAcks, bool shouldProcessAcks) :
_timeTracker(tt),
_shouldSendAcks(shouldSendAcks),
_shouldProcessAcks(shouldProcessAcks),
_nextOutgoingSequenceNumber(0),
_nextExpectedSequenceNumber(0),
_deliveredPacketCount(0),
_droppedPacketCount(0),
_dispatchedPacketCount(0)
{
    // Empty
}

InFlightPacket* DeliveryNotificationManager::writeState(OutputMemoryBitStream& ombs)
{
    InFlightPacket* ret = writeSequenceNumber(ombs);
    if (_shouldSendAcks)
    {
        writeAckData(ombs);
    }
    
    return ret;
}

bool DeliveryNotificationManager::readAndProcessState(InputMemoryBitStream& imbs)
{
    bool ret = processSequenceNumber(imbs);
    if (_shouldProcessAcks)
    {
        processAcks(imbs);
    }
    
    return ret;
}

void DeliveryNotificationManager::processTimedOutPackets(uint32_t time)
{
    uint32_t timeoutTime = time - NW_ACK_TIMEOUT;
    
    while (!_inFlightPackets.empty())
    {
        const auto& nextInFlightPacket = _inFlightPackets.front();
        
        if (nextInFlightPacket.getTimeDispatched() < timeoutTime)
        {
            handlePacketDeliveryFailure(nextInFlightPacket);
            _inFlightPackets.pop_front();
        }
        else
        {
            break;
        }
    }
}

uint32_t DeliveryNotificationManager::getDroppedPacketCount() const
{
    return _droppedPacketCount;
}

uint32_t DeliveryNotificationManager::getDeliveredPacketCount() const
{
    return _deliveredPacketCount;
}

uint32_t DeliveryNotificationManager::getDispatchedPacketCount() const
{
    return _dispatchedPacketCount;
}

const std::deque<InFlightPacket>& DeliveryNotificationManager::getInFlightPackets() const
{
    return _inFlightPackets;
}

void DeliveryNotificationManager::logStats()
{
    if (_dispatchedPacketCount > 0 && _shouldProcessAcks)
    {
        LOG("DeliveryNotificationManager stats: delivery rate %d%%, drop rate %d%%",
            (100 * _deliveredPacketCount) / _dispatchedPacketCount,
            (100 * _droppedPacketCount) / _dispatchedPacketCount);
    }
}

InFlightPacket* DeliveryNotificationManager::writeSequenceNumber(OutputMemoryBitStream& ombs)
{
    uint16_t sequenceNumber = _nextOutgoingSequenceNumber++;
    ombs.write(sequenceNumber);
    
    ++_dispatchedPacketCount;
    
    if (_shouldProcessAcks)
    {
        _inFlightPackets.emplace_back(sequenceNumber, _timeTracker->_time);
        
        return &_inFlightPackets.back();
    }
    
    return NULL;
}

void DeliveryNotificationManager::writeAckData(OutputMemoryBitStream& ombs)
{
    bool hasAcks = (_pendingAcks.size() > 0);
    
    ombs.write(hasAcks);
    if (hasAcks)
    {
        _pendingAcks.front().write(ombs);
        _pendingAcks.pop_front();
    }
}

bool DeliveryNotificationManager::processSequenceNumber(InputMemoryBitStream& imbs)
{
    uint16_t sequenceNumber;
    
    imbs.read(sequenceNumber);
    if (sequenceNumber == _nextExpectedSequenceNumber)
    {
        _nextExpectedSequenceNumber = sequenceNumber + 1;
        if (_shouldSendAcks)
        {
            addPendingAck(sequenceNumber);
        }
        return true;
    }
    else if (sequenceNumber < _nextExpectedSequenceNumber)
    {
        return false;
    }
    else if (sequenceNumber > _nextExpectedSequenceNumber)
    {
        _nextExpectedSequenceNumber = sequenceNumber + 1;
        if (_shouldSendAcks)
        {
            addPendingAck(sequenceNumber);
        }
        return true;
    }
    
    return false;
}

void DeliveryNotificationManager::processAcks(InputMemoryBitStream& imbs)
{
    bool hasAcks;
    imbs.read(hasAcks);
    if (hasAcks)
    {
        AckRange ackRange;
        ackRange.read(imbs);
        
        uint16_t nextAckdSequenceNumber = ackRange.getStart();
        uint32_t onePastAckdSequenceNumber = nextAckdSequenceNumber + ackRange.getCount();
        while (nextAckdSequenceNumber < onePastAckdSequenceNumber && !_inFlightPackets.empty())
        {
            const auto& nextInFlightPacket = _inFlightPackets.front();
            uint16_t nextInFlightuint16_t = nextInFlightPacket.getSequenceNumber();
            if (nextInFlightuint16_t < nextAckdSequenceNumber)
            {
                auto copyOfInFlightPacket = nextInFlightPacket;
                _inFlightPackets.pop_front();
                handlePacketDeliveryFailure(copyOfInFlightPacket);
            }
            else if (nextInFlightuint16_t == nextAckdSequenceNumber)
            {
                handlePacketDeliverySuccess(nextInFlightPacket);
                _inFlightPackets.pop_front();
                ++nextAckdSequenceNumber;
            }
            else if (nextInFlightuint16_t > nextAckdSequenceNumber)
            {
                ++nextAckdSequenceNumber;
            }
        }
    }
}

void DeliveryNotificationManager::addPendingAck(uint16_t inSequenceNumber)
{
    if (_pendingAcks.size() == 0 || !_pendingAcks.back().extendIfShould(inSequenceNumber))
    {
        _pendingAcks.push_back(inSequenceNumber);
    }
}

void DeliveryNotificationManager::handlePacketDeliveryFailure(const InFlightPacket& inFlightPacket)
{
    ++_droppedPacketCount;
    inFlightPacket.handleDeliveryFailure(this);
}

void DeliveryNotificationManager::handlePacketDeliverySuccess(const InFlightPacket& inFlightPacket)
{
    ++_deliveredPacketCount;
    inFlightPacket.handleDeliverySuccess(this);
}
