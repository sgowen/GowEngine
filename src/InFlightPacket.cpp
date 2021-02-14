//
//  InFlightPacket.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "InFlightPacket.hpp"

#include "TransmissionData.hpp"
#include "DeliveryNotificationManager.hpp"

#include "TimeTracker.hpp"

InFlightPacket::InFlightPacket(uint16_t sequenceNumber, uint32_t timeDispatched) :
_sequenceNumber(sequenceNumber),
_timeDispatched(timeDispatched),
_key(0)
{
    // Empty
}

InFlightPacket::~InFlightPacket()
{
    if (_key == 0)
    {
        return;
    }
    
    TransmissionData* td = getTransmissionData(_key);
    if (td != NULL)
    {
        td->free();
    }
}

void InFlightPacket::setTransmissionData(int key, TransmissionData* transmissionData)
{
    _key = key;
    
    _transmissionDataMap[key] = transmissionData;
}

TransmissionData* InFlightPacket::getTransmissionData(int key) const
{
    auto it = _transmissionDataMap.find(key);
    
    return it != _transmissionDataMap.end() ? it->second : NULL;
}

uint16_t InFlightPacket::getSequenceNumber() const
{
    return _sequenceNumber;
}

uint32_t InFlightPacket::getTimeDispatched() const
{
    return _timeDispatched;
}

void InFlightPacket::handleDeliveryFailure(DeliveryNotificationManager* dnm) const
{
    for (const auto& pair : _transmissionDataMap)
    {
        pair.second->handleDeliveryFailure(dnm);
    }
}

void InFlightPacket::handleDeliverySuccess(DeliveryNotificationManager* dnm) const
{
    for (const auto& pair : _transmissionDataMap)
    {
        pair.second->handleDeliverySuccess(dnm);
    }
}
