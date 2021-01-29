//
//  InFlightPacket.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <stdint.h>

class TransmissionData;
class DeliveryNotificationManager;

class InFlightPacket
{
public:
    InFlightPacket(uint16_t sequenceNumber, float timeDispatched);
    ~InFlightPacket();
    
    void setTransmissionData(int key, TransmissionData* transmissionData);
    TransmissionData* getTransmissionData(int key) const;
    uint16_t getSequenceNumber() const;
    float getTimeDispatched() const;
    void handleDeliveryFailure(DeliveryNotificationManager* dnm) const;
    void handleDeliverySuccess(DeliveryNotificationManager* dnm) const;
    
private:
    uint16_t _sequenceNumber;
    float _timeDispatched;
    int _key;
    
    std::map<int, TransmissionData*> _transmissionDataMap;
};
