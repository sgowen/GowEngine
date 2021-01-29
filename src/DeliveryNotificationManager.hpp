//
//  DeliveryNotificationManager.hpp
//  GowEngine
//
//  Created by Stephen Gowen on 5/15/17.
//  Copyright © 2021 Stephen Gowen. All rights reserved.
//

#pragma once

#include "InFlightPacket.hpp"
#include "AckRange.hpp"

#include <deque>
#include <stdint.h>

class Timing;
class OutputMemoryBitStream;
class InputMemoryBitStream;

class DeliveryNotificationManager
{
public:
    DeliveryNotificationManager(Timing* timing, bool shouldSendAcks, bool shouldProcessAcks);
    ~DeliveryNotificationManager();
    
    InFlightPacket* writeState(OutputMemoryBitStream& outputStream);
    bool readAndProcessState(InputMemoryBitStream& inputStream);
    void processTimedOutPackets(float frameStartTime);
    uint32_t getDroppedPacketCount() const;
    uint32_t getDeliveredPacketCount() const;
    uint32_t getDispatchedPacketCount() const;
    const std::deque<InFlightPacket>& getInFlightPackets() const;
    
private:
    Timing* _timing;
    bool _shouldSendAcks;
    bool _shouldProcessAcks;
    std::deque<InFlightPacket> _inFlightPackets;
    std::deque<AckRange> _pendingAcks;
    uint16_t _nextOutgoingSequenceNumber;
    uint16_t _nextExpectedSequenceNumber;
    uint32_t _deliveredPacketCount;
    uint32_t _droppedPacketCount;
    uint32_t _dispatchedPacketCount;
    
    InFlightPacket* writeSequenceNumber(OutputMemoryBitStream& outputStream);
    void writeAckData(OutputMemoryBitStream& outputStream);
    bool processSequenceNumber(InputMemoryBitStream& inputStream);
    void processAcks(InputMemoryBitStream& inputStream);
    void addPendingAck(uint16_t inSequenceNumber);
    void handlePacketDeliveryFailure(const InFlightPacket& inFlightPacket);
    void handlePacketDeliverySuccess(const InFlightPacket& inFlightPacket);
};
