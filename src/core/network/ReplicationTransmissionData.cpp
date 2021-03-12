//
//  ReplicationTransmissionData.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include <GowEngine/GowEngine.hpp>

ReplicationTransmissionData::ReplicationTransmissionData() :
_replicationManagerServer(NULL),
_entityRegistry(NULL),
_poolRMTD(NULL)
{
    // Empty
}

void ReplicationTransmissionData::free()
{
    if (_poolRMTD != NULL)
    {
        _poolRMTD->free(this);
    }
}

void ReplicationTransmissionData::handleDeliveryFailure(DeliveryNotificationManager* dnm) const
{
    for (const ReplicationTransmission& rt: _transmissions)
    {
        uint32_t networkID = rt.getID();
        
        switch (rt.getAction())
        {
            case REPA_CREATE:
                handleCreateDeliveryFailure(networkID);
                break;
            case REPA_UPDATE:
                handleUpdateStateDeliveryFailure(networkID, rt.dirtyState(), dnm);
                break;
            case REPA_DESTROY:
                handleDestroyDeliveryFailure(networkID);
                break;
        }
    }
}

void ReplicationTransmissionData::handleDeliverySuccess(DeliveryNotificationManager* dnm) const
{
    for (const ReplicationTransmission& rt: _transmissions)
    {
        switch (rt.getAction())
        {
            case REPA_CREATE:
                handleCreateDeliverySuccess(rt.getID());
                break;
            case REPA_DESTROY:
                handleDestroyDeliverySuccess(rt.getID());
                break;
            case REPA_UPDATE:
                break;
        }
    }
}

void ReplicationTransmissionData::reset(ReplicationManagerServer* rms, EntityRegistry* er, Pool<ReplicationTransmissionData>* poolRMTD)
{
    _replicationManagerServer = rms;
    _entityRegistry = er;
    _poolRMTD = poolRMTD;
    _transmissions.clear();
}

void ReplicationTransmissionData::addTransmission(uint32_t networkID, ReplicationAction ra, uint8_t dirtyState)
{
    _transmissions.emplace_back(networkID, ra, dirtyState);
}

void ReplicationTransmissionData::handleCreateDeliveryFailure(uint32_t networkID) const
{
    assert(_replicationManagerServer != NULL);
    assert(_entityRegistry != NULL);
    
    Entity* e = _entityRegistry->getEntityByID(networkID);
    if (e != NULL)
    {
        _replicationManagerServer->replicateCreate(networkID, ALL_DIRTY_STATE);
    }
}

void ReplicationTransmissionData::handleUpdateStateDeliveryFailure(uint32_t networkID, uint8_t dirtyState, DeliveryNotificationManager* dnm) const
{
    assert(_replicationManagerServer != NULL);
    assert(_entityRegistry != NULL);
    
    if (_entityRegistry->getEntityByID(networkID) != NULL)
    {
        for (const auto& inFlightPacket: dnm->getInFlightPackets())
        {
            ReplicationTransmissionData* rtd = static_cast<ReplicationTransmissionData*>(inFlightPacket.getTransmissionData('RPLM'));
            
            for (const ReplicationTransmission& otherRT: rtd->_transmissions)
            {
                dirtyState &= ~otherRT.dirtyState();
            }
        }
        
        if (dirtyState > 0)
        {
            _replicationManagerServer->setStateDirty(networkID, dirtyState);
        }
    }
}

void ReplicationTransmissionData::handleDestroyDeliveryFailure(uint32_t networkID) const
{
    _replicationManagerServer->replicateDestroy(networkID);
}

void ReplicationTransmissionData::handleCreateDeliverySuccess(uint32_t networkID) const
{
    _replicationManagerServer->handleCreateAckd(networkID);
}

void ReplicationTransmissionData::handleDestroyDeliverySuccess(uint32_t networkID) const
{
    _replicationManagerServer->removeFromReplication(networkID);
}

ReplicationTransmissionData::ReplicationTransmission::ReplicationTransmission(uint32_t networkID, ReplicationAction ra, uint8_t dirtyState) :
_networkID(networkID),
_action(ra),
_dirtyState(dirtyState)
{
    // Empty
}

int ReplicationTransmissionData::ReplicationTransmission::getID() const
{
    return _networkID;
}

ReplicationAction ReplicationTransmissionData::ReplicationTransmission::getAction() const
{
    return _action;
}

uint8_t ReplicationTransmissionData::ReplicationTransmission::dirtyState() const
{
    return _dirtyState;
}
