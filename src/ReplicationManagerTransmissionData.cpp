//
//  ReplicationManagerTransmissionData.cpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#include "ReplicationManagerTransmissionData.hpp"

#include "ReplicationManagerServer.hpp"

#include "EntityManager.hpp"
#include "Entity.hpp"
#include "ReplicationAction.hpp"
#include "DeliveryNotificationManager.hpp"

ReplicationManagerTransmissionData::ReplicationManagerTransmissionData() :
_replicationManagerServer(NULL)
{
    // Empty
}

void ReplicationManagerTransmissionData::free()
{
    if (_replicationManagerTransmissionDatas)
    {
        _replicationManagerTransmissionDatas->free(this);
    }
}

void ReplicationManagerTransmissionData::handleDeliveryFailure(DeliveryNotificationManager* dnm) const
{
    //run through the transmissions
    for (const ReplicationTransmission& rt: _transmissions)
    {
        //is it a create? then we have to redo the create.
        uint32_t networkID = rt.getID();
        
        switch(rt.getAction())
        {
            case REPA_CREATE:
                handleCreateDeliveryFailure(networkID);
                break;
            case REPA_UPDATE:
                handleUpdateStateDeliveryFailure(networkID, rt.state(), dnm);
                break;
            case REPA_DESTROY:
                handleDestroyDeliveryFailure(networkID);
                break;
        }
    }
}

void ReplicationManagerTransmissionData::handleDeliverySuccess(DeliveryNotificationManager* dnm) const
{
    //run through the transmissions, if any are Destroyed then we can remove this network id from the map
    for (const ReplicationTransmission& rt: _transmissions)
    {
        switch(rt.getAction())
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

void ReplicationManagerTransmissionData::reset(ReplicationManagerServer* replicationManagerServer, EntityManager* entityManager, Pool<ReplicationManagerTransmissionData>* replicationManagerTransmissionDatas)
{
    _replicationManagerServer = replicationManagerServer;
    _entityManager = entityManager;
    _replicationManagerTransmissionDatas = replicationManagerTransmissionDatas;
    _transmissions.clear();
}

void ReplicationManagerTransmissionData::addTransmission(uint32_t networkID, ReplicationAction ra, uint32_t state)
{
    _transmissions.push_back(ReplicationTransmission(networkID, ra, state));
}

void ReplicationManagerTransmissionData::handleCreateDeliveryFailure(uint32_t networkID) const
{
    assert(_replicationManagerServer);
    assert(_entityManager);
    
    //does the object still exist? it might be dead, in which case we don't resend a create
    Entity* entity = _entityManager->getEntityByID(networkID);
    if (entity)
    {
        _replicationManagerServer->replicateCreate(networkID, ALL_DIRTY_STATE);
    }
}

void ReplicationManagerTransmissionData::handleUpdateStateDeliveryFailure(uint32_t networkID, uint32_t state, DeliveryNotificationManager* dnm) const
{
    assert(_replicationManagerServer);
    assert(_entityManager);
    
    //does the object still exist? it might be dead, in which case we don't resend an update
    if (_entityManager->getEntityByID(networkID))
    {
        //look in all future in flight packets, in all transmissions
        //remove written state from dirty state
        for (const auto& inFlightPacket: dnm->getInFlightPackets())
        {
            ReplicationManagerTransmissionData* rmtdp = static_cast<ReplicationManagerTransmissionData*>(inFlightPacket.getTransmissionData('RPLM'));
            
            for (const ReplicationTransmission& otherRT: rmtdp->_transmissions)
            {
                state &= ~otherRT.state();
            }
        }
        
        //if there's still any dirty state, mark it
        if (state)
        {
            _replicationManagerServer->setStateDirty(networkID, state);
        }
    }
}

void ReplicationManagerTransmissionData::handleDestroyDeliveryFailure(uint32_t networkID) const
{
    _replicationManagerServer->replicateDestroy(networkID);
}

void ReplicationManagerTransmissionData::handleCreateDeliverySuccess(uint32_t networkID) const
{
    //we've received an ack for the create, so we can start sending as only an update
    _replicationManagerServer->handleCreateAckd(networkID);
}

void ReplicationManagerTransmissionData::handleDestroyDeliverySuccess(uint32_t networkID) const
{
    _replicationManagerServer->removeFromReplication(networkID);
}

ReplicationManagerTransmissionData::ReplicationTransmission::ReplicationTransmission(uint32_t networkID, ReplicationAction ra, uint32_t state) :
_networkID(networkID),
_action(ra),
_state(state)
{
    // Empty
}

int ReplicationManagerTransmissionData::ReplicationTransmission::getID() const
{
    return _networkID;
}

ReplicationAction ReplicationManagerTransmissionData::ReplicationTransmission::getAction() const
{
    return _action;
}

uint32_t ReplicationManagerTransmissionData::ReplicationTransmission::state() const
{
    return _state;
}
