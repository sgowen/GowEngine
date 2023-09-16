//
//  InFlightPacket.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include <map>
#include <stdint.h>

class TransmissionData;
class DeliveryNotificationManager;

class InFlightPacket
{
public:
    InFlightPacket(uint16_t sequenceNumber, uint32_t timeDispatched);
    ~InFlightPacket();
    
    void setTransmissionData(int key, TransmissionData* transmissionData);
    TransmissionData* getTransmissionData(int key) const;
    uint16_t getSequenceNumber() const;
    uint32_t getTimeDispatched() const;
    void handleDeliveryFailure(DeliveryNotificationManager* dnm) const;
    void handleDeliverySuccess(DeliveryNotificationManager* dnm) const;
    
private:
    uint16_t _sequenceNumber;
    uint32_t _timeDispatched;
    int _key;
    
    std::map<int, TransmissionData*> _transmissionDatas;
};
