//
//  TransmissionData.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

class DeliveryNotificationManager;

class TransmissionData
{
public:
    virtual void free() = 0;
    virtual void handleDeliveryFailure(DeliveryNotificationManager* dnm) const = 0;
    virtual void handleDeliverySuccess(DeliveryNotificationManager* dnm) const = 0;
    
protected:
    TransmissionData() {}
    virtual ~TransmissionData() {}
};
