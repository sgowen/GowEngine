//
//  ReplicationManagerTransmissionData.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "TransmissionData.hpp"

#include "ReplicationAction.hpp"
#include "Pool.hpp"

#include <vector>

class ReplicationManagerServer;
class EntityManager;

class ReplicationManagerTransmissionData : public TransmissionData
{
public:
    ReplicationManagerTransmissionData();
    
    virtual void free() override;
    virtual void handleDeliveryFailure(DeliveryNotificationManager* dnm) const override;
    virtual void handleDeliverySuccess(DeliveryNotificationManager* dnm) const override;
    
    void reset(ReplicationManagerServer* replicationManagerServer, EntityManager* entityManager, Pool<ReplicationManagerTransmissionData>* replicationManagerTransmissionDatas);
    void addTransmission(uint32_t networkID, ReplicationAction ra, uint32_t state);
    
private:
    class ReplicationTransmission;
    
    ReplicationManagerServer* _replicationManagerServer;
    EntityManager* _entityManager;
    Pool<ReplicationManagerTransmissionData>* _replicationManagerTransmissionDatas;
    std::vector<ReplicationTransmission> _transmissions;
    
    void handleCreateDeliveryFailure(uint32_t networkID) const;
    void handleUpdateStateDeliveryFailure(uint32_t networkID, uint32_t state, DeliveryNotificationManager* dnm) const;
    void handleDestroyDeliveryFailure(uint32_t networkID) const;
    void handleCreateDeliverySuccess(uint32_t networkID) const;
    void handleDestroyDeliverySuccess(uint32_t networkID) const;
    
    class ReplicationTransmission
    {
    public:
        ReplicationTransmission(uint32_t networkID, ReplicationAction ra, uint32_t state);
        
        int getID() const;
        ReplicationAction getAction() const;
        uint32_t state() const;
        
    private:
        int _networkID;
        ReplicationAction _action;
        uint32_t _state;
    };
};
