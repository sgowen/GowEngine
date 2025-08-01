//
//  ReplicationTransmissionData.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2023 Stephen Gowen. All rights reserved.
//

#pragma once

#include "core/common/Pool.hpp"

#include "TransmissionData.hpp"
#include "ReplicationCommand.hpp"

#include <vector>

class ReplicationManagerServer;
class EntityRegistry;

class ReplicationTransmissionData : public TransmissionData
{
public:
    ReplicationTransmissionData();
    
    virtual void free() override;
    virtual void handleDeliveryFailure(DeliveryNotificationManager* dnm) const override;
    virtual void handleDeliverySuccess(DeliveryNotificationManager* dnm) const override;
    
    void reset(ReplicationManagerServer* rms, EntityRegistry* er, Pool<ReplicationTransmissionData>* poolRMTD);
    void addTransmission(uint32_t networkID, ReplicationAction ra, uint8_t dirtyState);
    
private:
    class ReplicationTransmission;
    
    ReplicationManagerServer* _replicationManagerServer;
    EntityRegistry* _entityRegistry;
    Pool<ReplicationTransmissionData>* _poolRMTD;
    std::vector<ReplicationTransmission> _transmissions;
    
    void handleCreateDeliveryFailure(uint32_t networkID) const;
    void handleUpdateStateDeliveryFailure(uint32_t networkID, uint8_t dirtyState, DeliveryNotificationManager* dnm) const;
    void handleDestroyDeliveryFailure(uint32_t networkID) const;
    void handleCreateDeliverySuccess(uint32_t networkID) const;
    void handleDestroyDeliverySuccess(uint32_t networkID) const;
    
    class ReplicationTransmission
    {
    public:
        ReplicationTransmission(uint32_t networkID, ReplicationAction ra, uint8_t dirtyState);
        
        int getID() const;
        ReplicationAction getAction() const;
        uint8_t dirtyState() const;
        
    private:
        int _networkID;
        ReplicationAction _action;
        uint8_t _dirtyState;
    };
};
